from datetime import datetime, timezone
from sqlalchemy import Column, Integer, String, ForeignKey
from app.core.database import Base

class WorkspaceInvite(Base):
    __tablename__ = "workspace_invites"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    workspace_id = Column(Integer, ForeignKey("workspaces.id", ondelete="CASCADE"), nullable=False)
    inviter_id   = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    invitee_id   = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    role         = Column(String, default="member")
    status       = Column(String, default="pending")  # pending / accepted / rejected
    created_at   = Column(String, default=lambda: datetime.now(timezone.utc).isoformat())
