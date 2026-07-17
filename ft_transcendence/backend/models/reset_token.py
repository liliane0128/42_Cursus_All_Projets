from sqlalchemy import Column, String, Integer, Boolean
from app.core.database import Base

class ResetToken(Base):
    __tablename__ = "reset_tokens"
    id         = Column(Integer, primary_key=True, autoincrement=True)
    user_id    = Column(Integer, nullable=False)
    token      = Column(String, unique=True, nullable=False)
    used       = Column(Boolean, default=False)
    created_at = Column(String, server_default="now()")
