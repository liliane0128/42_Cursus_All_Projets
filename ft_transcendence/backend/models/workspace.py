from sqlalchemy import Column, Integer, String, Text, Boolean
from app.core.database import Base

class Workspace(Base):
    __tablename__ = "workspaces"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    name         = Column(String, nullable=False)
    description  = Column(Text, default="")
    color        = Column(String, default="#6366f1")
    deadline     = Column(String, nullable=True)   # ISO date string e.g. "2026-06-30"
    is_completed = Column(Boolean, default=False, nullable=False)
    created_at   = Column(String, server_default="now()")


class WorkspaceFile(Base):
    __tablename__ = "workspace_files"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    workspace_id = Column(Integer, nullable=False, index=True)
    uploader_id  = Column(Integer, nullable=True)
    name         = Column(String, nullable=False)
    size_str     = Column(String, nullable=False)
    file_path    = Column(Text, nullable=False)
    created_at   = Column(String, server_default="now()")
