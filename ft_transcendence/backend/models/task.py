from sqlalchemy import Column, Integer, String, ForeignKey
from app.core.database import Base

class Task(Base):
    __tablename__ = "tasks"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    title        = Column(String, nullable=False)
    status       = Column(String, default="todo")
    priority     = Column(String, default="medium")
    workspace_id = Column(Integer, ForeignKey("workspaces.id", ondelete="CASCADE"), nullable=True)
    due_date     = Column(String, nullable=True)
    created_at   = Column(String, server_default="now()")
