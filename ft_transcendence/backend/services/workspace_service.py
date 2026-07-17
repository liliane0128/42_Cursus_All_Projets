import httpx
from fastapi import HTTPException
from sqlalchemy.orm import Session
from typing import Optional, List

from models.workspace import Workspace
from models.workspace_member import WorkspaceMember
from models.workspace_notification import WorkspaceNotification

CHAT_SERVER = "http://chat:3001"


def _push_ws(user_id: int, notif_type: str, message: str, workspace_id: Optional[int]):
    try:
        httpx.post(f"{CHAT_SERVER}/internal/push", json={
            "userId": str(user_id),
            "event": {
                "type": "workspace:notification",
                "payload": {
                    "notif_type": notif_type,
                    "message": message,
                    "workspace_id": workspace_id,
                },
            },
        }, timeout=1.0)
    except Exception:
        pass


def require_auth(caller_id: Optional[int]) -> int:
    if not caller_id:
        raise HTTPException(401, "Authentication required")
    return caller_id


def get_workspace_or_404(db: Session, ws_id: int) -> Workspace:
    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    if not ws:
        raise HTTPException(404, "Workspace not found")
    return ws


def get_member(db: Session, ws_id: int, user_id: int) -> Optional[WorkspaceMember]:
    return db.query(WorkspaceMember).filter(
        WorkspaceMember.workspace_id == ws_id,
        WorkspaceMember.user_id == user_id,
    ).first()


def require_member(db: Session, ws_id: int, user_id: int) -> WorkspaceMember:
    m = get_member(db, ws_id, user_id)
    if not m:
        raise HTTPException(403, "Not a member of this workspace")
    return m


def require_owner_or_admin(db: Session, ws_id: int, user_id: int) -> WorkspaceMember:
    m = get_member(db, ws_id, user_id)
    if not m or m.role not in ("owner", "admin"):
        raise HTTPException(403, "Only owner or admin can perform this action")
    return m


def require_owner(db: Session, ws_id: int, user_id: int) -> WorkspaceMember:
    m = get_member(db, ws_id, user_id)
    if not m or m.role != "owner":
        raise HTTPException(403, "Only the workspace owner can perform this action")
    return m


def notify_members(
    db: Session,
    workspace_id: int,
    notif_type: str,
    message: str,
    exclude_ids: Optional[List[int]] = None,
):
    members = db.query(WorkspaceMember).filter(WorkspaceMember.workspace_id == workspace_id).all()
    for m in members:
        if exclude_ids and m.user_id in exclude_ids:
            continue
        db.add(WorkspaceNotification(
            user_id=m.user_id,
            workspace_id=workspace_id,
            type=notif_type,
            message=message,
        ))
        _push_ws(m.user_id, notif_type, message, workspace_id)


def notify_user(
    db: Session,
    user_id: int,
    workspace_id: Optional[int],
    notif_type: str,
    message: str,
):
    db.add(WorkspaceNotification(
        user_id=user_id,
        workspace_id=workspace_id,
        type=notif_type,
        message=message,
    ))
    _push_ws(user_id, notif_type, message, workspace_id)


def push_ws_only(user_id: int, notif_type: str, message: str, workspace_id: Optional[int]):
    """Push a real-time WS event without creating a DB record (e.g. invite signal)."""
    _push_ws(user_id, notif_type, message, workspace_id)


def ws_out(w: Workspace) -> dict:
    return {
        "id": w.id, "name": w.name,
        "description": w.description, "color": w.color,
        "deadline": w.deadline,
        "is_completed": w.is_completed or False,
        "created_at": w.created_at,
    }
