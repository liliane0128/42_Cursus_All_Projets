from sqlalchemy import Column, String, Text, Boolean
from app.core.database import Base

class Message(Base):
    __tablename__ = "messages"
    id          = Column(String, primary_key=True)
    chat_id     = Column(String, nullable=False)
    sender_id   = Column(String, nullable=False)
    sender_name = Column(String, nullable=False)
    content     = Column(Text, nullable=False)
    type        = Column(String, default="user")
    created_at  = Column(String, nullable=False)
    edited      = Column(Boolean, default=False)
