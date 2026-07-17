from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from sqlalchemy import or_, and_
from app.core.database import get_db
from app.core.security import get_current_user
from models.friendship import Friendship
from models.user import User

router = APIRouter()


@router.post("/request/{user_id}")
def send_request(user_id: int, me=Depends(get_current_user), db: Session = Depends(get_db)):
    if user_id == me.id:
        raise HTTPException(400, "Cannot send request to yourself")
    existing = db.query(Friendship).filter(
        or_(
            and_(Friendship.requester_id == me.id, Friendship.addressee_id == user_id),
            and_(Friendship.requester_id == user_id, Friendship.addressee_id == me.id),
        ),
        Friendship.status.in_(["pending", "accepted"]),
    ).first()
    if existing:
        raise HTTPException(409, "Friendship already exists or pending")
    db.add(Friendship(requester_id=me.id, addressee_id=user_id, status="pending"))
    db.commit()
    return {"ok": True}


@router.post("/accept/{user_id}")
def accept_request(user_id: int, me=Depends(get_current_user), db: Session = Depends(get_db)):
    f = db.query(Friendship).filter(
        Friendship.requester_id == user_id,
        Friendship.addressee_id == me.id,
        Friendship.status == "pending",
    ).first()
    if not f:
        raise HTTPException(404, "Friend request not found")
    f.status = "accepted"
    db.commit()
    return {"ok": True}


@router.get("/requests")
def get_requests(me=Depends(get_current_user), db: Session = Depends(get_db)):
    rows = db.query(Friendship).filter(
        Friendship.addressee_id == me.id,
        Friendship.status == "pending",
    ).all()
    ids = [r.requester_id for r in rows]
    users = db.query(User).filter(User.id.in_(ids)).all()
    return [
        {
            "id": u.id, "login": u.login,
            "displayName": u.display_name,
            "avatar": u.avatar,
            "is_deleted": u.is_deleted,
        }
        for u in users
    ]


@router.get("")
def get_friends(me=Depends(get_current_user), db: Session = Depends(get_db)):
    rows = db.query(Friendship).filter(
        or_(Friendship.requester_id == me.id, Friendship.addressee_id == me.id),
        Friendship.status == "accepted",
    ).all()
    friend_ids = [
        r.addressee_id if r.requester_id == me.id else r.requester_id
        for r in rows
    ]
    users = db.query(User).filter(User.id.in_(friend_ids)).all()
    return [
        {
            "id": u.id,
            "login": u.login,
            "displayName": u.display_name,
            "avatar": u.avatar,
            "status": u.status or "offline",
            "is_deleted": u.is_deleted,
        }
        for u in users
    ]


@router.delete("/{user_id}")
def remove_friend(user_id: int, me=Depends(get_current_user), db: Session = Depends(get_db)):
    db.query(Friendship).filter(
        or_(
            and_(Friendship.requester_id == me.id, Friendship.addressee_id == user_id),
            and_(Friendship.requester_id == user_id, Friendship.addressee_id == me.id),
        )
    ).delete(synchronize_session=False)
    db.commit()
    return {"ok": True}
