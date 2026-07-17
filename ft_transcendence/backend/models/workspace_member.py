from sqlalchemy import Column, Integer, String, ForeignKey, UniqueConstraint
from app.core.database import Base

class WorkspaceMember(Base):
    __tablename__ = "workspace_members"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    workspace_id = Column(Integer, ForeignKey("workspaces.id", ondelete="CASCADE"), nullable=False)
    user_id      = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    role         = Column(String, default="member")  # "owner", "admin", "member"
    __table_args__ = (UniqueConstraint("workspace_id", "user_id", name="uq_ws_user"),)
