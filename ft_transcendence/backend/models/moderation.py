from sqlalchemy import Column, Integer, String, Float
from app.core.database import Base

class ModerationRecord(Base):
    __tablename__ = "moderation_records"
    user_id   = Column(String, primary_key=True)
    warnings  = Column(Integer, default=0)
    ban_until = Column(Float, default=0.0)
