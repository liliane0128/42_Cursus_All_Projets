from fastapi import APIRouter, Request
from fastapi.responses import StreamingResponse
from pydantic import BaseModel
from services.ai_service import get_ai_reply
from groq import Groq
import json
import os
import asyncio

router = APIRouter()

class ChatRequest(BaseModel):
    user_id: str
    message: str
    chat_id: str = "ai"
    user_name: str = ""
    user_message_id: str = ""
    model: str = "llama-3.3-70b-versatile"

class ChatResponse(BaseModel):
    reply: str

@router.post("/chat", response_model=ChatResponse)
async def chat(request: ChatRequest):
    print(f"AI chat request: user_id={request.user_id}, chat_id={request.chat_id}")
    print(f"user_name received: {request.user_name}")
    reply = await get_ai_reply(request.user_id, request.message, request.chat_id, request.user_name, request.user_message_id, request.model)
    return ChatResponse(reply=reply)

@router.post("/stream")
async def chat_stream(request: ChatRequest):
    async def generate():
        from services.ai_service import _rate_limit_lock, _request_counts, conversation_history, SYSTEM_PROMPT
        from datetime import datetime, timezone

        now = datetime.now(timezone.utc).timestamp()
        with _rate_limit_lock:
            _request_counts[request.user_id] = [t for t in _request_counts[request.user_id] if now - t < 60]
            if len(_request_counts[request.user_id]) >= 10:
                yield f"data: {json.dumps({'type': 'error', 'content': '⚠️ Too many requests. Please wait a moment.'})}\n\n"
                return
            _request_counts[request.user_id].append(now)

        # Content moderation
        from services.ai_service import get_moderation_record, save_moderation_record
        from app.core.database import SessionLocal as _SessionLocal
        import time as _time

        _mod_db = _SessionLocal()
        try:
            _record = get_moderation_record(_mod_db, request.user_id)
            if _record.ban_until > _time.time():
                remaining = int(_record.ban_until - _time.time())
                minutes = remaining // 60
                seconds = remaining % 60
                yield f"data: {json.dumps({'type': 'error', 'content': f'🚫 You are banned from AI features for {minutes}m {seconds}s due to repeated violations.'})}\n\n"
                return

            try:
                from groq import Groq as _GroqMod
                _mod_client = _GroqMod(api_key=os.getenv("GROQ_API_KEY"))
                mod_resp = _mod_client.chat.completions.create(
                    model="llama-3.3-70b-versatile",
                    messages=[
                        {"role": "system", "content": "You are a content moderator. Reply with only 'SAFE' or 'UNSAFE'. Then if UNSAFE, add a colon and briefly explain why in one sentence."},
                        {"role": "user", "content": request.message}
                    ],
                    max_tokens=60,
                )
                mod_result = mod_resp.choices[0].message.content.strip()
                print(f"[moderation stream] result: {repr(mod_result)}", flush=True)
                if mod_result.upper().startswith("UNSAFE"):
                    _record.warnings += 1
                    warning_count = _record.warnings
                    if warning_count >= 3:
                        _record.ban_until = _time.time() + 3600
                        save_moderation_record(_mod_db, _record)
                        msg = f"🚫 You have been banned from AI features for 1 hour. {mod_result.split(':', 1)[1].strip() if ':' in mod_result else ''}"
                    else:
                        save_moderation_record(_mod_db, _record)
                        warning_text = mod_result.split(':', 1)[1].strip() if ':' in mod_result else "Your message violates community guidelines."
                        msg = f"⚠️ Warning {warning_count}/3: {warning_text}"
                    yield f"data: {json.dumps({'type': 'error', 'content': msg})}\n\n"
                    return
            except Exception as mod_err:
                print(f"[moderation stream] error: {mod_err}", flush=True)
        finally:
            _mod_db.close()

        from langchain_core.messages import HumanMessage, AIMessage
        history_key = f"{request.chat_id}:{request.user_id}"
        history = conversation_history.get(history_key, [])

        messages = [{"role": "system", "content": SYSTEM_PROMPT}]
        for msg in history:
            if isinstance(msg, HumanMessage):
                messages.append({"role": "user", "content": msg.content})
            elif isinstance(msg, AIMessage):
                messages.append({"role": "assistant", "content": msg.content})
        messages.append({"role": "user", "content": request.message})

        client = Groq(api_key=os.getenv("GROQ_API_KEY"))
        full_reply = ""
        try:
            stream = client.chat.completions.create(
                model=request.model,
                messages=messages,
                stream=True,
                max_tokens=1000,
            )
            for chunk in stream:
                delta = chunk.choices[0].delta.content or ""
                if delta:
                    full_reply += delta
                    yield f"data: {json.dumps({'type': 'chunk', 'content': delta})}\n\n"
                    await asyncio.sleep(0)
        except Exception as e:
            yield f"data: {json.dumps({'type': 'error', 'content': 'Sorry, something went wrong.'})}\n\n"
            return

        if history_key not in conversation_history:
            conversation_history[history_key] = []
        conversation_history[history_key].append(HumanMessage(content=request.message))
        conversation_history[history_key].append(AIMessage(content=full_reply))

        if "<function(" in full_reply or "tool_call" in full_reply or "</function>" in full_reply:
            yield f"data: {json.dumps({'type': 'done', 'content': 'Sorry, I encountered an internal error. Please try again.'})}\n\n"
            return

        yield f"data: {json.dumps({'type': 'done', 'content': full_reply})}\n\n"

    return StreamingResponse(generate(), media_type="text/event-stream", headers={
        "Cache-Control": "no-cache",
        "X-Accel-Buffering": "no",
        "Transfer-Encoding": "chunked",
    })
