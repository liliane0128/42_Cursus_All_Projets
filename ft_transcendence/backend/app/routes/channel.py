from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import get_current_user
from models.workspace import Workspace
from models.workspace_member import WorkspaceMember
from models.user import User

router = APIRouter()


@router.get("")
def get_channels(db: Session = Depends(get_db), current_user=Depends(get_current_user)):
    member_rows = db.query(WorkspaceMember).filter(
        WorkspaceMember.user_id == current_user.id
    ).all()
    workspace_ids = [r.workspace_id for r in member_rows]
    if not workspace_ids:
        return []
    workspaces = db.query(Workspace).filter(Workspace.id.in_(workspace_ids)).all()
    result = []
    for ws in workspaces:
        members = db.query(WorkspaceMember).filter(
            WorkspaceMember.workspace_id == ws.id
        ).all()
        members_with_users = (
            db.query(WorkspaceMember, User)
            .join(User, User.id == WorkspaceMember.user_id)
            .filter(WorkspaceMember.workspace_id == ws.id)
            .all()
        )
        result.append({
            "id": f"workspace_{ws.id}",
            "name": ws.name,
            "description": ws.description or "",
            "color": ws.color or "#6366f1",
            "member_ids": [m.user_id for m in members],
            "members": [
                {"id": m.user_id, "display_name": user.display_name, "avatar": user.avatar, "role": m.role}
                for m, user in members_with_users
            ],
        })
    return result
