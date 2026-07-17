from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import get_current_user
from models.link import Link
import uuid
from datetime import datetime, timezone

router = APIRouter()

class LinkIn(BaseModel):
    name: str
    url: str

def link_out(l: Link):
    return { "id": l.id, "chat_id": l.chat_id, "name": l.name, "url": l.url, "created_at": l.created_at }

@router.get("/{chat_id}")
def get_links(chat_id: str, db: Session = Depends(get_db), _=Depends(get_current_user)):
    links = db.query(Link).filter(Link.chat_id == chat_id).order_by(Link.created_at.asc()).all()
    return [link_out(l) for l in links]

@router.post("/{chat_id}")
def add_link(chat_id: str, body: LinkIn, db: Session = Depends(get_db), _=Depends(get_current_user)):
    link = Link(
        id=str(uuid.uuid4()),
        chat_id=chat_id,
        name=body.name,
        url=body.url,
        created_at=datetime.now(timezone.utc).isoformat(),
    )
    db.add(link)
    db.commit()
    db.refresh(link)
    return link_out(link)

@router.delete("/{link_id}")
def delete_link(link_id: str, db: Session = Depends(get_db), _=Depends(get_current_user)):
    link = db.query(Link).filter(Link.id == link_id).first()
    if not link:
        raise HTTPException(404, "Link not found")
    db.delete(link)
    db.commit()
    return {"ok": True}
