from sqlalchemy import Column, Integer, String, ForeignKey
from app.core.database import Base

class Friendship(Base):
    __tablename__ = "friendships"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    requester_id = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    addressee_id = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    status       = Column(String, default="pending")  # pending / accepted / rejected
    created_at   = Column(String, server_default="now()")
