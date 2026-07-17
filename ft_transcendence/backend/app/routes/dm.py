from fastapi import APIRouter, Depends, HTTPException, Request
from sqlalchemy.orm import Session
from sqlalchemy import and_, or_
from app.core.database import get_db
from app.core.security import get_current_user
from models.dm_channel import DmChannel
from models.friendship import Friendship
from models.message import Message
from models.user import User

router = APIRouter()


@router.get("/check/{chat_id}")
def check_dm_allowed(chat_id: str, request: Request, db: Session = Depends(get_db)):
    try:
        if not chat_id.startswith("dm-"):
            return {"allowed": True}
        dm_id = int(chat_id[3:])
    except (ValueError, IndexError):
        return {"allowed": True}
    dm = db.query(DmChannel).filter(DmChannel.id == dm_id).first()
    if not dm:
        return {"allowed": True}
    try:
        sender_id = int(request.headers.get("x-sender-id", "0"))
    except ValueError:
        return {"allowed": True}
    recipient_id = dm.user2_id if sender_id == dm.user1_id else dm.user1_id
    recipient = db.query(User).filter(User.id == recipient_id).first()
    if recipient and recipient.is_deleted:
        return {"allowed": False, "recipient_deleted": True}
    friendship = db.query(Friendship).filter(
        and_(
            or_(
                and_(Friendship.requester_id == dm.user1_id, Friendship.addressee_id == dm.user2_id),
                and_(Friendship.requester_id == dm.user2_id, Friendship.addressee_id == dm.user1_id),
            ),
            Friendship.status == "accepted",
        )
    ).first()
    return {"allowed": bool(friendship)}


@router.delete("/{chat_id}/messages")
def delete_dm_messages(chat_id: str, me=Depends(get_current_user), db: Session = Depends(get_db)):
    try:
        if not chat_id.startswith("dm-"):
            raise HTTPException(400, "Invalid chat_id format")
        dm_id = int(chat_id[3:])
    except ValueError:
        raise HTTPException(400, "Invalid chat_id format")
    dm = db.query(DmChannel).filter(DmChannel.id == dm_id).first()
    if not dm:
        raise HTTPException(404, "DM channel not found")
    if me.id not in (dm.user1_id, dm.user2_id):
        raise HTTPException(403, "Not a participant of this DM")
    db.query(Message).filter(Message.chat_id == chat_id).delete()
    db.commit()
    return {"ok": True}


@router.post("/{user_id}")
def get_or_create_dm(user_id: int, me=Depends(get_current_user), db: Session = Depends(get_db)):
    if user_id == me.id:
        raise HTTPException(400, "Cannot DM yourself")
    other = db.query(User).filter(User.id == user_id).first()
    if not other:
        raise HTTPException(404, "User not found")
    u1, u2 = min(me.id, user_id), max(me.id, user_id)
    dm = db.query(DmChannel).filter(
        DmChannel.user1_id == u1, DmChannel.user2_id == u2
    ).first()
    if not dm:
        dm = DmChannel(user1_id=u1, user2_id=u2)
        db.add(dm)
        db.commit()
        db.refresh(dm)
    return {"chat_id": f"dm-{dm.id}", "user_id": other.id, "display_name": other.display_name}


@router.get("")
def list_dms(me=Depends(get_current_user), db: Session = Depends(get_db)):
    rows = db.query(DmChannel).filter(
        or_(DmChannel.user1_id == me.id, DmChannel.user2_id == me.id)
    ).all()
    result = []
    for dm in rows:
        other_id = dm.user2_id if dm.user1_id == me.id else dm.user1_id
        other = db.query(User).filter(User.id == other_id).first()
        if other:
            result.append({
                "chat_id": f"dm-{dm.id}",
                "user_id": other.id,
                "display_name": other.display_name,
                "is_deleted": other.is_deleted,
            })
    return result
