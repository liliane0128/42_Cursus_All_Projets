from langchain_groq import ChatGroq
from langchain_core.messages import HumanMessage, AIMessage, SystemMessage, ToolMessage
from langchain_core.tools import tool
from dotenv import load_dotenv
from app.core.database import SessionLocal
from models.message import Message
from models.task import Task
from collections import defaultdict
import threading
import asyncio
import re
import os
import uuid
from datetime import datetime, timezone

import time

# Rate limiting: max 10 requests per user per minute
_rate_limit_lock = threading.Lock()
_request_counts: dict = defaultdict(list)  # user_id -> [timestamp, ...]

def get_moderation_record(db, user_id: str):
    from models.moderation import ModerationRecord
    record = db.query(ModerationRecord).filter(ModerationRecord.user_id == user_id).first()
    if not record:
        record = ModerationRecord(user_id=user_id, warnings=0, ban_until=0.0)
        db.add(record)
        db.commit()
        db.refresh(record)
    return record

def save_moderation_record(db, record):
    db.commit()

load_dotenv()

llm = ChatGroq(
    model="llama-3.3-70b-versatile",
    api_key=os.getenv("GROQ_API_KEY"),
    max_tokens=300,
)

SYSTEM_PROMPT = """You are Meowlinette, an AI assistant of a real-time collaboration platform for 42 school students.
Guidelines:
- Be concise, clear, and helpful
- Be warm and friendly, but keep a professional tone
- Do not use cat expressions unless the user brings it up first
- Answer in the same language the user writes in
- For technical topics (C, C++, Python, FastAPI, React, Docker, WebSocket, 42 projects), give precise and practical answers
- Keep responses under 3 sentences for simple questions. Only elaborate when the topic requires it
- When a user asks to create a task or check milestone status, use the available tools instead of describing what you would do
- Only use tools (create_task, get_tasks) when the user explicitly asks to create a task or check tasks. Do not call tools for greetings or general conversation.
- Detect the emotional tone of the user's message (happy, frustrated, sad, excited, neutral)
- Adapt your response accordingly: mirror positive energy, show empathy for frustration or sadness, stay calm for anger. Add a single relevant emoji at the end of your response to reflect the emotional tone (😊 happy/warm, 😄 excited/great news, 🥺 empathy/sad, 😔 disappointed/sorry, 😤 frustrated, 😠 angry/confronting discrimination or hate speech, 😡 very angry/strong disapproval, 😰 stressed/worried, 🤩 amazing/wow, 😌 calm/reassured, 🥳 celebrating, 😅 awkward/oops, 🤔 thinking/uncertain)
- If you don't know something, say so directly. Do not invent information about the project or team
- You are embedded in the platform and have context about the project, but you cannot access live data unless tools are provided

Tool usage rules:
- create_task: call when user explicitly asks to create/add a task
- get_tasks: call when user asks to see/list/check tasks, OR automatically before update_task/delete_task to find the task id by name
- update_task: call when user asks to modify a task's status, priority, title, or due date. Always get_tasks first if you don't have the id
- delete_task: call when user asks to delete/remove a task. Always get_tasks first if you don't have the id
- Never mention task ids in responses
- Never show function call syntax in responses
- Do not call tools for greetings or general conversation
- Perform tool lookups silently, only report the final result"""

conversation_history: dict[str, list] = {}

@tool
def create_task(title: str, workspace_id: int = None, status: str = "todo", priority: str = "medium", due_date: str = None, user_id: int = None, chat_id: str = None) -> str:
    """Create a task in the project board. Requires a title. Optionally accepts workspace_id, status, priority, due_date."""
    print(f"[create_task] called: title={title!r}, workspace_id={workspace_id}, status={status}, priority={priority}, due_date={due_date}, user_id={user_id}, chat_id={chat_id}")
    try:
        from models.workspace import Workspace
        db = SessionLocal()
        try:
            if workspace_id is None:
                if chat_id is not None:
                    if chat_id and chat_id.startswith("workspace_"):
                        try:
                            ws_id = int(chat_id.replace("workspace_", ""))
                            matched = db.query(Workspace).filter(Workspace.id == ws_id).first()
                        except ValueError:
                            matched = None
                    else:
                        matched = db.query(Workspace).filter(
                            Workspace.name.ilike(f"%{chat_id}%")
                        ).first()
                    if matched:
                        workspace_id = matched.id
                        print(f"[create_task] resolved workspace_id={workspace_id} by chat_id ilike match '{chat_id}'")
                    else:
                        print(f"[create_task] no workspace matched chat_id='{chat_id}', inserting with workspace_id=None")
                else:
                    print("[create_task] no chat_id provided, inserting with workspace_id=None")
            task = Task(title=title, workspace_id=workspace_id, status=status, priority=priority, due_date=due_date)
            db.add(task)
            print(f"[create_task] committing task title={task.title!r} workspace_id={workspace_id}")
            db.commit()
            db.refresh(task)
            print(f"[create_task] committed successfully, task.id={task.id}")
            return f"Task created: title='{task.title}', status={task.status}, priority={task.priority}"
        except Exception as inner_e:
            db.rollback()
            print(f"[create_task] DB error, rolled back: {inner_e}")
            raise
        finally:
            db.close()
    except Exception as e:
        print(f"[create_task] failed: {e}")
        return f"Failed to create task: {e}"

@tool
def get_tasks(workspace_id: int = None) -> str:
    """Get the list of tasks from the project board. Optionally filter by workspace_id."""
    try:
        db = SessionLocal()
        try:
            q = db.query(Task)
            if workspace_id:
                q = q.filter(Task.workspace_id == workspace_id)
            tasks = q.order_by(Task.id.desc()).all()
            from models.workspace import Workspace
            valid_ws_ids = {ws.id for ws in db.query(Workspace).all()}
            tasks = [t for t in tasks if t.workspace_id in valid_ws_ids]
            if not tasks:
                return "No tasks found."
            lines = [f"- [{t.status}] {t.title} (id={t.id}, priority={t.priority})" for t in tasks]
            return f"{len(tasks)} task(s):\n" + "\n".join(lines)
        finally:
            db.close()
    except Exception as e:
        return f"Failed to fetch tasks: {e}"


@tool
def delete_task(task_id: str) -> str:
    """Delete a task from the project board by its id (accepts string or integer). IMPORTANT: task_id must be an integer. If you only have the task name, call get_tasks first to get the integer id."""
    try:
        task_id = int(task_id)
        db = SessionLocal()
        try:
            task = db.query(Task).filter(Task.id == task_id).first()
            if not task:
                return f"Task id={task_id} not found."
            title = task.title
            db.delete(task)
            db.commit()
            return f"Task deleted: title='{title}'"
        finally:
            db.close()
    except Exception as e:
        return f"Failed to delete task: {e}"

@tool
def update_task(task_id: str, status: str = None, title: str = None, priority: str = None, due_date: str = None) -> str:
    """Update a task's status, title, priority, or due date. task_id must be the task's TITLE (string) if you don't have the integer id yet — the system will automatically look up the id by title. status must be one of: 'todo', 'in_progress', 'done'. priority must be one of: low, medium, high."""
    print(f"[update_task] called: task_id={task_id}, status={status}, priority={priority}, due_date={due_date}")
    try:
        task_id = int(task_id)
        db = SessionLocal()
        try:
            task = db.query(Task).filter(Task.id == task_id).first()
            if not task:
                return f"Task id={task_id} not found."
            if status is not None:
                task.status = status
            if title is not None:
                task.title = title
            if priority is not None:
                task.priority = priority
            if due_date is not None:
                task.due_date = due_date
            db.commit()
            print(f"[update_task] committed successfully, task.id={task.id}")
            db.refresh(task)
            return f"Task updated: title='{task.title}', status={task.status}, priority={task.priority}, due_date={task.due_date}"
        finally:
            db.close()
    except Exception as e:
        print(f"[update_task] failed: {e}")
        return f"Failed to update task: {e}"

tools = [create_task, get_tasks, delete_task, update_task]
llm_with_tools = llm.bind_tools(tools)

async def get_ai_reply(user_id: str, message: str, chat_id: str = "ai", user_name: str = "", user_message_id: str = "", model: str = "llama-3.3-70b-versatile") -> str:
    # Rate limiting check
    now = datetime.now(timezone.utc).timestamp()
    with _rate_limit_lock:
        _request_counts[user_id] = [t for t in _request_counts[user_id] if now - t < 60]
        if len(_request_counts[user_id]) >= 10:
            return "⚠️ Too many requests. Please wait a moment before trying again."
        _request_counts[user_id].append(now)

    # Content moderation
    _mod_db = SessionLocal()
    try:
        record = get_moderation_record(_mod_db, user_id)
        if record.ban_until > time.time():
            remaining = int(record.ban_until - time.time())
            minutes = remaining // 60
            seconds = remaining % 60
            return f"🚫 You are banned from using AI features for {minutes}m {seconds}s due to repeated violations of community guidelines."

        try:
            from groq import Groq as GroqClient
            _mod_client = GroqClient(api_key=os.getenv("GROQ_API_KEY"))
            mod_resp = _mod_client.chat.completions.create(
                model="llama-3.3-70b-versatile",
                messages=[
                    {"role": "system", "content": "You are a content moderator. First reply with 'SAFE' or 'UNSAFE', then if UNSAFE, briefly explain in one sentence why (e.g. 'UNSAFE: This message contains racist language targeting...'). Be concise."},
                    {"role": "user", "content": message}
                ],
                max_tokens=60,
            )
            mod_result = mod_resp.choices[0].message.content.strip()
            print(f"[moderation] result: {repr(mod_result)}", flush=True)
            if mod_result.upper().startswith("UNSAFE"):
                record.warnings += 1
                warning_count = record.warnings
                if warning_count >= 3:
                    record.ban_until = time.time() + 3600
                    save_moderation_record(_mod_db, record)
                    return f"🚫 You have been banned from AI features for 1 hour due to repeated violations. {mod_result.split(':', 1)[1].strip() if ':' in mod_result else ''}"
                save_moderation_record(_mod_db, record)
                warning_text = mod_result.split(':', 1)[1].strip() if ':' in mod_result else "Your message violates community guidelines."
                return f"⚠️ Warning {warning_count}/3: {warning_text} Further violations will result in a ban."
        except Exception as mod_err:
            print(f"[moderation] error: {mod_err}", flush=True)
    finally:
        _mod_db.close()

    history_key = f"{chat_id}:{user_id}"
    if history_key not in conversation_history:
        db = SessionLocal()
        try:
            rows = (
                db.query(Message)
                .filter(Message.chat_id == chat_id)
                .order_by(Message.created_at.asc())
                .all()
            )
            loaded: list = []
            for row in rows:
                if row.type == "user":
                    loaded.append(HumanMessage(content=row.content))
                elif row.type == "ai":
                    loaded.append(AIMessage(content=row.content))
            conversation_history[history_key] = loaded
        finally:
            db.close()

    history = conversation_history[history_key]

    _llm = ChatGroq(model=model, api_key=os.getenv("GROQ_API_KEY"), max_tokens=300)
    _llm_with_tools = _llm.bind_tools(tools)

    messages = [SystemMessage(content=SYSTEM_PROMPT), SystemMessage(content=f"Current date and time: {datetime.now().strftime('%Y-%m-%d %H:%M')}")]
    messages += history
    messages.append(HumanMessage(content=message))

    try:
        response = await _llm_with_tools.ainvoke(messages)

        # If the bot decides to call a tool
        if response.tool_calls:
            follow_up = [SystemMessage(content=SYSTEM_PROMPT)] + history + [HumanMessage(content=message), response]
            for tool_call in response.tool_calls:
                if tool_call["name"] == "create_task":
                    tool_call_func = create_task
                    tool_call["args"].setdefault("user_id", int(user_id) if user_id else None)
                    tool_call["args"].setdefault("chat_id", chat_id)
                elif tool_call["name"] == "get_tasks":
                    tool_call_func = get_tasks
                elif tool_call["name"] in ("delete_task", "update_task"):
                    tool_call_func = delete_task if tool_call["name"] == "delete_task" else update_task
                    raw_id = str(tool_call["args"].get("task_id", ""))
                    if not raw_id.isdigit():
                        tasks_result = await asyncio.to_thread(get_tasks.invoke, {"chat_id": chat_id})
                        print(f"[{tool_call['name']}] get_tasks result: {tasks_result}")
                        matched_id = None
                        for line in tasks_result.splitlines():
                            m = re.match(r"-\s+\[.*?\]\s+(.*?)\s+\(id=(\d+)", line)
                            if m and raw_id.lower() in m.group(1).lower():
                                matched_id = int(m.group(2))
                                break
                        if matched_id is not None:
                            print(f"[{tool_call['name']}] resolved task_id={matched_id} from title={raw_id!r}")
                            tool_call["args"]["task_id"] = str(matched_id)
                        else:
                            print(f"[{tool_call['name']}] could not resolve task_id from title={raw_id!r}")
                else:
                    tool_call_func = None
                if tool_call_func is not None:
                    result = await asyncio.to_thread(tool_call_func.invoke, tool_call["args"])
                else:
                    result = "Unknown tool"
                follow_up.append(ToolMessage(content=result, tool_call_id=tool_call["id"]))

            final_response = await _llm.ainvoke(follow_up)
            reply = final_response.content
            if "<function(" in reply or "tool_call" in reply or "</function>" in reply:
                print(f"[get_ai_reply] suspicious reply filtered: {reply[:200]}")
                return "Sorry, I encountered an internal error. Please try again."
        else:
            reply = response.content

        history.append(HumanMessage(content=message))
        history.append(AIMessage(content=reply))

        if len(history) > 20:
            conversation_history[history_key] = history[-20:]

        now = datetime.now(timezone.utc).isoformat()
        db = SessionLocal()
        try:
            db.add(Message(
                id=user_message_id or str(uuid.uuid4()),
                chat_id=chat_id,
                sender_id=user_id,
                sender_name=user_name or user_id,
                content=message,
                type="user",
                created_at=now,
            ))
            db.add(Message(
                id=str(uuid.uuid4()),
                chat_id=chat_id,
                sender_id="ai",
                sender_name="Meowlinette",
                content=reply,
                type="ai",
                created_at=datetime.now(timezone.utc).isoformat(),
            ))
            db.commit()
        except Exception as db_err:
            db.rollback()
            print(f"DB write error: {db_err}")
        finally:
            db.close()

        return reply

    except Exception as e:
        print(f"LangChain error: {e}")
        return "Meow. I could not reach the AI API right now."