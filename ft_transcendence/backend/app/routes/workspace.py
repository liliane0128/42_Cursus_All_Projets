import os, uuid, mimetypes
from fastapi import APIRouter, HTTPException, Depends, Security, UploadFile, File
from fastapi.responses import FileResponse
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from pydantic import BaseModel
from typing import Optional
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.config import UPLOAD_DIR, WORKSPACE_FILES_DIR
from app.core.security import decode_token
from models.workspace import Workspace, WorkspaceFile
from models.workspace_member import WorkspaceMember
from models.user import User
from services.workspace_service import (
    require_auth, get_workspace_or_404,
    get_member, require_member, require_owner_or_admin, require_owner,
    notify_members, notify_user, push_ws_only, ws_out,
)

router = APIRouter()
optional_bearer = HTTPBearer(auto_error=False)


# ── Pydantic schemas ──────────────────────────────────────────────────────────

class WorkspaceCreate(BaseModel):
    name: str
    description: str = ""
    color: str = "#6366f1"
    deadline: Optional[str] = None


class WorkspaceUpdate(BaseModel):
    name: Optional[str] = None
    description: Optional[str] = None
    color: Optional[str] = None
    deadline: Optional[str] = None
    is_completed: Optional[bool] = None


class InviteBody(BaseModel):
    login_or_email: str
    role: str = "member"


class RoleUpdate(BaseModel):
    role: str  # "admin" or "member"


class TransferBody(BaseModel):
    new_owner_id: int


# ── Auth dependency ───────────────────────────────────────────────────────────

def get_optional_user_id(
    creds: Optional[HTTPAuthorizationCredentials] = Security(optional_bearer),
) -> Optional[int]:
    if not creds:
        return None
    try:
        return decode_token(creds.credentials)
    except Exception:
        return None


# ── Workspace CRUD ────────────────────────────────────────────────────────────

@router.get("")
def get_workspaces(
    db: Session = Depends(get_db),
    user_id: Optional[int] = Depends(get_optional_user_id),
):
    if not user_id:
        return []
    workspaces = (
        db.query(Workspace)
        .join(WorkspaceMember, WorkspaceMember.workspace_id == Workspace.id)
        .filter(WorkspaceMember.user_id == user_id)
        .order_by(Workspace.id.desc())
        .all()
    )
    return [ws_out(w) for w in workspaces]


@router.post("")
def create_workspace(
    body: WorkspaceCreate,
    db: Session = Depends(get_db),
    user_id: Optional[int] = Depends(get_optional_user_id),
):
    ws = Workspace(**body.model_dump())
    db.add(ws)
    db.commit()
    db.refresh(ws)
    if user_id:
        db.add(WorkspaceMember(workspace_id=ws.id, user_id=user_id, role="owner"))
        db.commit()
    return ws_out(ws)


@router.patch("/{ws_id}")
def update_workspace(
    ws_id: int,
    body: WorkspaceUpdate,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    ws = get_workspace_or_404(db, ws_id)
    if caller_id:
        require_owner_or_admin(db, ws_id, caller_id)

    old_completed = ws.is_completed
    for k, v in body.model_dump(exclude_none=True).items():
        setattr(ws, k, v)

    if body.is_completed is not None and body.is_completed != old_completed:
        caller_user = db.query(User).filter(User.id == caller_id).first() if caller_id else None
        actor = caller_user.login if caller_user else "Someone"
        if body.is_completed:
            notify_members(db, ws_id, "workspace_completed",
                           f'{actor} marked workspace "{ws.name}" as completed',
                           exclude_ids=[caller_id] if caller_id else None)
        else:
            notify_members(db, ws_id, "workspace_reopened",
                           f'{actor} reopened workspace "{ws.name}"',
                           exclude_ids=[caller_id] if caller_id else None)

    db.commit()
    db.refresh(ws)
    return ws_out(ws)


@router.delete("/{ws_id}")
def delete_workspace(
    ws_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    if not ws:
        return {"ok": True}
    if caller_id:
        require_owner(db, ws_id, caller_id)
    notify_members(db, ws_id, "workspace_deleted", f'Workspace "{ws.name}" has been deleted')
    db.delete(ws)
    db.commit()
    return {"ok": True}


# ── Member management ─────────────────────────────────────────────────────────

@router.get("/{ws_id}/members")
def get_members(ws_id: int, db: Session = Depends(get_db)):
    rows = (
        db.query(WorkspaceMember, User)
        .join(User, WorkspaceMember.user_id == User.id)
        .filter(WorkspaceMember.workspace_id == ws_id, User.is_deleted == False)
        .all()
    )
    return [
        {
            "user_id": u.id,
            "login": u.login,
            "display_name": u.display_name,
            "avatar": u.avatar,
            "role": m.role,
        }
        for m, u in rows
    ]


@router.post("/{ws_id}/invite")
def invite_member(
    ws_id: int,
    body: InviteBody,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    from models.workspace_invite import WorkspaceInvite

    ws = get_workspace_or_404(db, ws_id)

    user = (
        db.query(User)
        .filter((User.login == body.login_or_email) | (User.email == body.login_or_email))
        .first()
    )
    if not user:
        raise HTTPException(404, "User not found")
    if caller_id and user.id == caller_id:
        raise HTTPException(400, "Cannot invite yourself")

    if db.query(WorkspaceMember).filter(
        WorkspaceMember.workspace_id == ws_id,
        WorkspaceMember.user_id == user.id,
    ).first():
        raise HTTPException(400, "User is already a member")

    if db.query(WorkspaceInvite).filter(
        WorkspaceInvite.workspace_id == ws_id,
        WorkspaceInvite.invitee_id == user.id,
        WorkspaceInvite.status == "pending",
    ).first():
        raise HTTPException(400, "A pending invitation already exists for this user")

    db.query(WorkspaceInvite).filter(
        WorkspaceInvite.workspace_id == ws_id,
        WorkspaceInvite.invitee_id == user.id,
    ).delete()

    if caller_id and not get_member(db, ws_id, caller_id):
        db.add(WorkspaceMember(workspace_id=ws_id, user_id=caller_id, role="owner"))

    role = body.role if body.role in ("member", "admin") else "member"
    db.add(WorkspaceInvite(
        workspace_id=ws_id,
        inviter_id=caller_id or 0,
        invitee_id=user.id,
        role=role,
        status="pending",
    ))
    db.commit()

    caller_user = db.query(User).filter(User.id == caller_id).first() if caller_id else None
    caller_name = caller_user.login if caller_user else "Someone"
    push_ws_only(user.id, "invite_received",
                 f'{caller_name} invited you to join "{ws.name}"', ws_id)

    return {"message": f"Invitation sent to {user.login}"}


@router.patch("/{ws_id}/members/{target_user_id}")
def update_member_role(
    ws_id: int,
    target_user_id: int,
    body: RoleUpdate,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    require_owner(db, ws_id, caller_id)

    target_m = get_member(db, ws_id, target_user_id)
    if not target_m:
        raise HTTPException(404, "Member not found")
    if target_m.role == "owner":
        raise HTTPException(400, "Cannot change the owner's role")
    if body.role not in ("admin", "member"):
        raise HTTPException(400, "Role must be 'admin' or 'member'")

    old_role = target_m.role
    target_m.role = body.role

    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    target_user = db.query(User).filter(User.id == target_user_id).first()
    ws_name = ws.name if ws else "workspace"
    if body.role == "admin" and old_role != "admin":
        msg = f"{target_user.login} has been promoted to Admin in \"{ws_name}\""
    else:
        msg = f"{target_user.login}'s admin role has been removed in \"{ws_name}\""
    notify_members(db, ws_id, "role_changed", msg)
    db.commit()
    return {"message": "Role updated"}


@router.delete("/{ws_id}/members/{target_user_id}")
def kick_member(
    ws_id: int,
    target_user_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    caller_m = require_owner_or_admin(db, ws_id, caller_id)

    target_m = get_member(db, ws_id, target_user_id)
    if not target_m:
        raise HTTPException(404, "Member not found")
    if target_m.role == "owner":
        raise HTTPException(403, "Cannot remove the workspace owner")
    if caller_m.role == "admin" and target_m.role == "admin":
        raise HTTPException(403, "Admin cannot remove other admins")

    target_user = db.query(User).filter(User.id == target_user_id).first()
    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    ws_name = ws.name if ws else "workspace"

    db.delete(target_m)
    notify_members(db, ws_id, "member_kicked", f"{target_user.login} was removed from \"{ws_name}\"",
                   exclude_ids=[target_user_id])
    notify_user(db, target_user_id, ws_id, "member_kicked",
                f"You were removed from workspace \"{ws_name}\"")
    db.commit()
    return {"ok": True}


@router.delete("/{ws_id}/leave")
def leave_workspace(
    ws_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)

    caller_m = get_member(db, ws_id, caller_id)
    if not caller_m:
        raise HTTPException(404, "You are not a member of this workspace")
    if caller_m.role == "owner":
        raise HTTPException(400, "Owner must transfer ownership before leaving")

    caller_user = db.query(User).filter(User.id == caller_id).first()
    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    ws_name = ws.name if ws else "workspace"

    db.delete(caller_m)
    notify_members(db, ws_id, "member_left",
                   f"{caller_user.login} left \"{ws_name}\"",
                   exclude_ids=[caller_id])
    db.commit()
    return {"ok": True}


@router.get("/{ws_id}/pending-invites")
def get_workspace_pending_invites(
    ws_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    from models.workspace_invite import WorkspaceInvite

    if caller_id:
        require_owner_or_admin(db, ws_id, caller_id)

    rows = (
        db.query(WorkspaceInvite, User)
        .join(User, WorkspaceInvite.invitee_id == User.id)
        .filter(WorkspaceInvite.workspace_id == ws_id, WorkspaceInvite.status == "pending")
        .order_by(WorkspaceInvite.id.desc())
        .all()
    )
    return [
        {
            "id": inv.id,
            "invitee_login": u.login,
            "invitee_display_name": u.display_name,
            "invitee_avatar": u.avatar,
            "role": inv.role,
            "created_at": inv.created_at,
        }
        for inv, u in rows
    ]


@router.delete("/{ws_id}/invites/{invite_id}")
def cancel_invite(
    ws_id: int,
    invite_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    from models.workspace_invite import WorkspaceInvite

    require_auth(caller_id)
    require_owner_or_admin(db, ws_id, caller_id)

    invite = db.query(WorkspaceInvite).filter(
        WorkspaceInvite.id == invite_id,
        WorkspaceInvite.workspace_id == ws_id,
        WorkspaceInvite.status == "pending",
    ).first()
    if not invite:
        raise HTTPException(404, "Invite not found")

    db.delete(invite)
    db.commit()
    return {"ok": True}


# ── Workspace files ───────────────────────────────────────────────────────────

ALLOWED_EXTENSIONS = {
    "jpg","jpeg","png","gif","webp","svg","ico","bmp",
    "pdf","md","mdx","rst","tex",
    "py","js","ts","tsx","jsx","html","htm","css","scss","sass","less","vue","svelte",
    "c","cpp","cc","cxx","h","hpp","java","kt","go","rs","rb","php","swift",
    "sh","bash","zsh","fish","ps1","bat",
    "json","yaml","yml","toml","ini","conf","env","xml","csv","tsv","sql","txt","log",
    "zip","tar","gz","bz2","xz","7z","tgz",
    "ipynb","dockerfile","makefile",
}


@router.get("/{ws_id}/files")
def list_files(
    ws_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    require_member(db, ws_id, caller_id)
    files = db.query(WorkspaceFile).filter(WorkspaceFile.workspace_id == ws_id).order_by(WorkspaceFile.id).all()
    return [
        {
            "id": f.id,
            "name": f.name,
            "size": f.size_str,
            "url": f"/api/workspaces/{ws_id}/files/{f.id}/download",
            "uploadedAt": f.created_at,
        }
        for f in files
    ]


@router.post("/{ws_id}/files")
async def upload_file(
    ws_id: int,
    file: UploadFile = File(...),
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    require_member(db, ws_id, caller_id)

    fname_lower = (file.filename or "").lower()
    ext = fname_lower.rsplit(".", 1)[-1] if "." in fname_lower else fname_lower
    if ext not in ALLOWED_EXTENSIONS and fname_lower not in ALLOWED_EXTENSIONS:
        raise HTTPException(415, f"File type '.{ext}' is not allowed")

    data = await file.read()
    if len(data) > 5 * 1024 * 1024:
        raise HTTPException(413, "File exceeds 5 MB limit")

    ws_dir = os.path.join(WORKSPACE_FILES_DIR, f"workspace_{ws_id}")
    os.makedirs(ws_dir, exist_ok=True)

    ext = file.filename.rsplit(".", 1)[-1] if file.filename and "." in file.filename else "bin"
    fname = f"{uuid.uuid4().hex}.{ext}"
    with open(os.path.join(ws_dir, fname), "wb") as fp:
        fp.write(data)

    size_bytes = len(data)
    size_str = f"{size_bytes} B" if size_bytes < 1024 else f"{size_bytes / 1024:.1f} KB"

    wf = WorkspaceFile(
        workspace_id=ws_id,
        uploader_id=caller_id,
        name=file.filename or fname,
        size_str=size_str,
        file_path=fname,
    )
    db.add(wf)
    db.commit()
    db.refresh(wf)
    return {
        "id": wf.id,
        "name": wf.name,
        "size": wf.size_str,
        "url": f"/api/workspaces/{ws_id}/files/{wf.id}/download",
        "uploadedAt": wf.created_at,
    }


@router.get("/{ws_id}/files/{file_id}/download")
def download_file(
    ws_id: int,
    file_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    require_member(db, ws_id, caller_id)

    wf = db.query(WorkspaceFile).filter(
        WorkspaceFile.id == file_id,
        WorkspaceFile.workspace_id == ws_id,
    ).first()
    if not wf:
        raise HTTPException(404, "File not found")

    disk_path = os.path.join(WORKSPACE_FILES_DIR, f"workspace_{ws_id}", wf.file_path)
    if not os.path.exists(disk_path):
        # fall back to old location for files uploaded before the migration
        disk_path = os.path.join(UPLOAD_DIR, f"workspace_{ws_id}", wf.file_path)
    if not os.path.exists(disk_path):
        raise HTTPException(404, "File not found on disk")

    media_type, _ = mimetypes.guess_type(wf.name)
    return FileResponse(
        path=disk_path,
        filename=wf.name,
        media_type=media_type or "application/octet-stream",
    )


@router.delete("/{ws_id}/files/{file_id}")
def delete_file(
    ws_id: int,
    file_id: int,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    require_member(db, ws_id, caller_id)

    wf = db.query(WorkspaceFile).filter(
        WorkspaceFile.id == file_id,
        WorkspaceFile.workspace_id == ws_id,
    ).first()
    if not wf:
        raise HTTPException(404, "File not found")

    disk_path = os.path.join(WORKSPACE_FILES_DIR, f"workspace_{ws_id}", wf.file_path)
    if not os.path.exists(disk_path):
        disk_path = os.path.join(UPLOAD_DIR, f"workspace_{ws_id}", wf.file_path)
    if os.path.exists(disk_path):
        os.remove(disk_path)

    db.delete(wf)
    db.commit()
    return {"ok": True}


@router.patch("/{ws_id}/transfer")
def transfer_ownership(
    ws_id: int,
    body: TransferBody,
    db: Session = Depends(get_db),
    caller_id: Optional[int] = Depends(get_optional_user_id),
):
    require_auth(caller_id)
    caller_m = require_owner(db, ws_id, caller_id)

    if body.new_owner_id == caller_id:
        raise HTTPException(400, "Already the owner")

    new_owner_m = get_member(db, ws_id, body.new_owner_id)
    if not new_owner_m:
        raise HTTPException(404, "Target user is not a member of this workspace")

    caller_user = db.query(User).filter(User.id == caller_id).first()
    new_owner_user = db.query(User).filter(User.id == body.new_owner_id).first()
    ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
    ws_name = ws.name if ws else "workspace"

    caller_m.role = "member"
    new_owner_m.role = "owner"
    notify_members(db, ws_id, "ownership_transferred",
                   f"{caller_user.login} transferred ownership of \"{ws_name}\" to {new_owner_user.login}")
    db.commit()
    return {"ok": True}
