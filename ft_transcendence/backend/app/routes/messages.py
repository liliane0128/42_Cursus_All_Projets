import re
from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from sqlalchemy.orm import Session
from sqlalchemy.exc import IntegrityError
from sqlalchemy import and_, or_
from app.core.database import get_db
from app.core.security import get_current_user
from models.message import Message
from models.dm_channel import DmChannel
from models.friendship import Friendship
from models.user import User

router = APIRouter()

BANNED_WORDS = [
    # English - most common profanity
    "fuck", "fucking", "fucked", "fucker",
    "shit", "shitty", "bullshit",
    "asshole", "ass",
    "bitch", "bitchy",
    "cunt", "dick", "cock", "pussy",
    "bastard", "motherfucker", "jackass",
    "damn", "hell",
    # English - hate speech & slurs
    "nigger", "nigga", "faggot", "fag",
    "retard", "retarded", "tranny",
    "dyke", "spic", "chink", "gook", "kike",
    "whore", "slut", "skank", "hoe", "thot",
    "bimbo", "nazi", "kys", "kill yourself",
    # French - most common profanity
    "putain", "merde", "connard", "connasse",
    "salope", "enculé", "enculer", "va te faire foutre",
    "bordel", "chier", "bite", "couille",
    "pute", "nique", "niquer", "batard", "bâtard",
    "ferme ta gueule", "ta gueule", "casse toi",
    # French - hate speech & slurs
    "pd", "gonzesse", "tapette", "bamboula",
    "nazi", "gros con",
]

class MessageIn(BaseModel):
    id: str
    chat_id: str
    sender_id: str
    sender_name: str
    content: str
    type: str = "user"
    created_at: str

def check_dm_permissions(db: Session, msg: MessageIn, sender_id: int):
    """Permission check for DM messages: friendship status and recipient not deactivated.
    Returns immediately if chat_id is not a DM."""
    if not msg.chat_id.startswith("dm-"):
        return
    try:
        dm_id = int(msg.chat_id[3:])
    except ValueError:
        raise HTTPException(400, "Invalid DM channel id")
    dm = db.query(DmChannel).filter(DmChannel.id == dm_id).first()
    if not dm:
        raise HTTPException(404, "DM channel not found")
    friendship = db.query(Friendship).filter(
        and_(
            or_(
                and_(Friendship.requester_id == dm.user1_id, Friendship.addressee_id == dm.user2_id),
                and_(Friendship.requester_id == dm.user2_id, Friendship.addressee_id == dm.user1_id),
            ),
            Friendship.status == "accepted",
        )
    ).first()
    if not friendship:
        raise HTTPException(403, "You are no longer friends. Remove this conversation or add them back to send messages.")
    recipient_id = dm.user2_id if sender_id == dm.user1_id else dm.user1_id
    recipient = db.query(User).filter(User.id == recipient_id).first()
    if recipient and recipient.is_deleted:
        raise HTTPException(403, "Cannot send message to a deactivated account")


def check_moderation(msg: MessageIn):
    """Check for banned words in non-AI messages."""
    if msg.type == "ai":
        return
    content_lower = msg.content.lower()
    for word in BANNED_WORDS:
        pattern = r'\b' + re.escape(word) + r'\b'
        if re.search(pattern, content_lower):
            raise HTTPException(400, "Message contains inappropriate content.")


@router.post("")
def save_message(msg: MessageIn, db: Session = Depends(get_db), _=Depends(get_current_user)):
    check_dm_permissions(db, msg, _.id)
    check_moderation(msg)

    existing = db.query(Message).filter(Message.id == msg.id).first()
    if existing:
        return {"ok": True, "duplicate": True}

    m = Message(**msg.model_dump())
    db.add(m)
    try:
        db.commit()
    except IntegrityError:
        db.rollback()
        return {"ok": True, "duplicate": True}
    return {"ok": True}

class MessageEdit(BaseModel):
    content: str

@router.patch("/{message_id}")
def edit_message(message_id: str, body: MessageEdit, db: Session = Depends(get_db), current_user=Depends(get_current_user)):
    msg = db.query(Message).filter(Message.id == message_id).first()
    if not msg:
        raise HTTPException(404, "Message not found")
    if str(msg.sender_id) != str(current_user.id):
        raise HTTPException(403, "Not allowed")
    msg.content = body.content
    msg.edited = True
    db.commit()
    return {"ok": True}

@router.delete("/{message_id}")
def delete_message(message_id: str, db: Session = Depends(get_db), current_user=Depends(get_current_user)):
    msg = db.query(Message).filter(Message.id == message_id).first()
    if not msg:
        raise HTTPException(404, "Message not found")
    if str(msg.sender_id) != str(current_user.id):
        raise HTTPException(403, "Not allowed")
    db.delete(msg)
    db.commit()
    return {"ok": True}

@router.get("/{chat_id}")
def get_messages(chat_id: str, db: Session = Depends(get_db), _=Depends(get_current_user)):
    msgs = db.query(Message).filter(Message.chat_id == chat_id).all()
    msgs.sort(key=lambda m: (m.created_at or ""), reverse=False)
    print(f'GET messages for {chat_id}: {len(msgs)} rows')
    return [{"id": m.id, "chat_id": m.chat_id, "sender_id": m.sender_id,
             "sender_name": m.sender_name, "content": m.content,
             "type": m.type, "created_at": m.created_at, "edited": m.edited} for m in msgs]
