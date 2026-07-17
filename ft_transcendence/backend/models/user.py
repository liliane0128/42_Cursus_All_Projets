from sqlalchemy import Column, Integer, String, Text, Boolean
from app.core.database import Base

class User(Base):
    __tablename__ = "users"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    login        = Column(String, unique=True, nullable=False)
    email        = Column(String, unique=True, nullable=True)
    display_name = Column(String, nullable=False)
    password     = Column(String, nullable=True)
    avatar       = Column(String, nullable=True)
    bio          = Column(Text, default="")
    status       = Column(String, default="offline")
    ft_id        = Column(Integer, unique=True, nullable=True)
    github_id    = Column(Integer, unique=True, nullable=True)
    totp_secret  = Column(String, nullable=True)
    totp_enabled = Column(Boolean, default=False)
    is_deleted   = Column(Boolean, default=False)
