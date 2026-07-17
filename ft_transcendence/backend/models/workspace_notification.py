from datetime import datetime, timezone
from sqlalchemy import Column, Integer, String, Text, Boolean, ForeignKey
from app.core.database import Base

class WorkspaceNotification(Base):
    __tablename__ = "workspace_notifications"
    id           = Column(Integer, primary_key=True, autoincrement=True)
    user_id      = Column(Integer, ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    workspace_id = Column(Integer, ForeignKey("workspaces.id", ondelete="SET NULL"), nullable=True)
    type         = Column(String, nullable=False)
    message      = Column(Text, nullable=False)
    is_read      = Column(Boolean, default=False)
    created_at   = Column(String, default=lambda: datetime.now(timezone.utc).isoformat())
