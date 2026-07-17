from fastapi import APIRouter, Depends
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import decode_token
from models.workspace_notification import WorkspaceNotification

router = APIRouter()
bearer = HTTPBearer()


def get_uid(creds: HTTPAuthorizationCredentials = Depends(bearer)) -> int:
    return decode_token(creds.credentials)


@router.get("")
def get_notifications(db: Session = Depends(get_db), user_id: int = Depends(get_uid)):
    notifs = (
        db.query(WorkspaceNotification)
        .filter(WorkspaceNotification.user_id == user_id)
        .order_by(WorkspaceNotification.id.desc())
        .limit(50)
        .all()
    )
    return [
        {
            "id": n.id,
            "workspace_id": n.workspace_id,
            "type": n.type,
            "message": n.message,
            "is_read": n.is_read,
            "created_at": n.created_at,
        }
        for n in notifs
    ]


@router.get("/unread-count")
def get_unread_count(db: Session = Depends(get_db), user_id: int = Depends(get_uid)):
    from models.workspace_invite import WorkspaceInvite
    ws_unread = db.query(WorkspaceNotification).filter(
        WorkspaceNotification.user_id == user_id,
        WorkspaceNotification.is_read == False,
    ).count()
    invite_count = db.query(WorkspaceInvite).filter(
        WorkspaceInvite.invitee_id == user_id,
        WorkspaceInvite.status == "pending",
    ).count()
    return {"count": ws_unread + invite_count}


@router.post("/read-all")
def mark_all_read(db: Session = Depends(get_db), user_id: int = Depends(get_uid)):
    db.query(WorkspaceNotification).filter(
        WorkspaceNotification.user_id == user_id,
        WorkspaceNotification.is_read == False,
    ).update({"is_read": True})
    db.commit()
    return {"ok": True}


@router.delete("")
def clear_notifications(db: Session = Depends(get_db), user_id: int = Depends(get_uid)):
    db.query(WorkspaceNotification).filter(
        WorkspaceNotification.user_id == user_id,
    ).delete()
    db.commit()
    return {"ok": True}
