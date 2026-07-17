from fastapi import APIRouter, HTTPException, Depends, Security
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from pydantic import BaseModel
from typing import Optional
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import decode_token
from models.task import Task
from models.workspace_member import WorkspaceMember

router = APIRouter()
optional_bearer = HTTPBearer(auto_error=False)


def get_optional_user_id(
    creds: Optional[HTTPAuthorizationCredentials] = Security(optional_bearer),
) -> Optional[int]:
    if not creds:
        return None
    try:
        return decode_token(creds.credentials)
    except Exception:
        return None

class TaskCreate(BaseModel):
    title: str
    workspace_id: int
    status: str = "todo"
    priority: str = "medium"
    due_date: Optional[str] = None

class TaskUpdate(BaseModel):
    title: Optional[str] = None
    status: Optional[str] = None
    priority: Optional[str] = None
    due_date: Optional[str] = None

def task_out(t: Task):
    return {
        "id": t.id, "title": t.title, "status": t.status,
        "priority": t.priority, "workspace_id": t.workspace_id,
        "due_date": t.due_date, "created_at": t.created_at,
    }

@router.get("")
def get_tasks(
    workspace_id: Optional[int] = None,
    db: Session = Depends(get_db),
    user_id: Optional[int] = Depends(get_optional_user_id),
):
    q = db.query(Task)
    if workspace_id:
        q = q.filter(Task.workspace_id == workspace_id)
    elif user_id:
        # Only return tasks belonging to workspaces the user is a member of
        member_ws_ids = db.query(WorkspaceMember.workspace_id).filter(
            WorkspaceMember.user_id == user_id
        ).subquery()
        q = q.filter(Task.workspace_id.in_(member_ws_ids))
    else:
        return []
    return [task_out(t) for t in q.order_by(Task.id.desc()).all()]

@router.post("")
def create_task(body: TaskCreate, db: Session = Depends(get_db)):
    duplicate = db.query(Task).filter(
        Task.workspace_id == body.workspace_id,
        Task.title == body.title,
    ).first()
    if duplicate:
        raise HTTPException(409, "A task with this name already exists in this workspace")
    task = Task(**body.model_dump())
    db.add(task)
    db.commit()
    db.refresh(task)
    return task_out(task)

@router.patch("/{task_id}")
def update_task(task_id: int, body: TaskUpdate, db: Session = Depends(get_db)):
    task = db.query(Task).filter(Task.id == task_id).first()
    if not task:
        raise HTTPException(404, "Task not found")
    if body.title and body.title != task.title:
        duplicate = db.query(Task).filter(
            Task.workspace_id == task.workspace_id,
            Task.title == body.title,
            Task.id != task_id,
        ).first()
        if duplicate:
            raise HTTPException(409, "A task with this name already exists in this workspace")
    for k, v in body.model_dump(exclude_none=True).items():
        setattr(task, k, v)
    db.commit()
    db.refresh(task)
    return task_out(task)

@router.delete("/{task_id}")
def delete_task(task_id: int, db: Session = Depends(get_db)):
    task = db.query(Task).filter(Task.id == task_id).first()
    if task:
        db.delete(task)
        db.commit()
    return {"ok": True}
