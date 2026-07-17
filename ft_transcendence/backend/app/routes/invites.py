from fastapi import APIRouter, HTTPException, Depends
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from pydantic import BaseModel
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import decode_token
from models.workspace_invite import WorkspaceInvite
from models.workspace_member import WorkspaceMember
from models.workspace import Workspace
from models.user import User
from services.workspace_service import notify_user, notify_members

router = APIRouter()
bearer = HTTPBearer()


def get_uid(creds: HTTPAuthorizationCredentials = Depends(bearer)) -> int:
    return decode_token(creds.credentials)


class RespondBody(BaseModel):
    action: str  # "accept" or "reject"


@router.get("/pending")
def get_pending_invites(db: Session = Depends(get_db), user_id: int = Depends(get_uid)):
    rows = (
        db.query(WorkspaceInvite, Workspace, User)
        .join(Workspace, WorkspaceInvite.workspace_id == Workspace.id)
        .join(User, WorkspaceInvite.inviter_id == User.id)
        .filter(WorkspaceInvite.invitee_id == user_id, WorkspaceInvite.status == "pending")
        .order_by(WorkspaceInvite.id.desc())
        .all()
    )
    return [
        {
            "id": inv.id,
            "workspace_id": inv.workspace_id,
            "workspace_name": ws.name,
            "inviter_login": inviter.login,
            "inviter_display_name": inviter.display_name,
            "role": inv.role,
            "created_at": inv.created_at,
        }
        for inv, ws, inviter in rows
    ]


@router.post("/{invite_id}/respond")
def respond_to_invite(
    invite_id: int,
    body: RespondBody,
    db: Session = Depends(get_db),
    user_id: int = Depends(get_uid),
):
    if body.action not in ("accept", "reject"):
        raise HTTPException(400, "Action must be 'accept' or 'reject'")

    invite = db.query(WorkspaceInvite).filter(
        WorkspaceInvite.id == invite_id,
        WorkspaceInvite.invitee_id == user_id,
        WorkspaceInvite.status == "pending",
    ).first()
    if not invite:
        raise HTTPException(404, "Invite not found or already responded")

    invitee = db.query(User).filter(User.id == user_id).first()
    ws = db.query(Workspace).filter(Workspace.id == invite.workspace_id).first()
    ws_name = ws.name if ws else "workspace"

    if body.action == "accept":
        existing = db.query(WorkspaceMember).filter(
            WorkspaceMember.workspace_id == invite.workspace_id,
            WorkspaceMember.user_id == user_id,
        ).first()
        if not existing:
            db.add(WorkspaceMember(workspace_id=invite.workspace_id, user_id=user_id, role=invite.role))
        invite.status = "accepted"
        # Notify all current members that someone joined
        notify_members(db, invite.workspace_id, "member_joined",
                       f"{invitee.login} joined \"{ws_name}\"",
                       exclude_ids=[user_id])
        db.commit()
        return {"message": "Invitation accepted", "workspace_id": invite.workspace_id}
    else:
        invite.status = "rejected"
        # Notify the inviter that their invitation was declined
        if invite.inviter_id:
            notify_user(db, invite.inviter_id, invite.workspace_id, "invite_declined",
                        f"{invitee.login} declined your invitation to \"{ws_name}\"")
        db.commit()
        return {"message": "Invitation rejected"}
