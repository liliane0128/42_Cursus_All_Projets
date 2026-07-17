import os, shutil, uuid, io, base64
from fastapi import APIRouter, Depends, HTTPException, UploadFile, File
from pydantic import BaseModel
from typing import Optional
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import get_current_user
from app.core.config import AVATAR_DIR
from models.user import User
from models.friendship import Friendship
from sqlalchemy import or_, and_
import pyotp
import qrcode

router = APIRouter()

def user_out(u: User):
    if u.is_deleted:
        return {
            "id": u.id, "login": u.login,
            "displayName": u.display_name, "email": None,
            "avatar": u.avatar, "bio": "",
            "status": "offline",
            "totp_enabled": False,
            "is_deleted": True,
        }
    return {
        "id": u.id, "login": u.login,
        "displayName": u.display_name, "email": u.email,
        "avatar": u.avatar, "bio": u.bio or "",
        "status": u.status or "offline",
        "totp_enabled": bool(u.totp_enabled),
        "is_deleted": False,
    }

class UpdateBody(BaseModel):
    displayName: Optional[str] = None
    bio: Optional[str] = None
    email: Optional[str] = None
    status: Optional[str] = None

@router.get("/me")
def get_me(user=Depends(get_current_user)):
    return user_out(user)

@router.patch("/me")
def update_me(body: UpdateBody, user=Depends(get_current_user), db: Session = Depends(get_db)):
    if body.status and body.status not in {"online", "offline", "in_game"}:
        raise HTTPException(400, "Invalid status")
    if body.displayName:
        user.display_name = body.displayName
    if body.email is not None:
        user.email = body.email
    if body.bio is not None:
        user.bio = body.bio
    if body.status:
        user.status = body.status
    db.commit()
    db.refresh(user)
    return user_out(user)

@router.post("/me/avatar")
async def upload_avatar(
    file: UploadFile = File(...),
    user=Depends(get_current_user),
    db: Session = Depends(get_db)
):
    if file.content_type not in ("image/jpeg", "image/png", "image/webp"):
        raise HTTPException(400, "Only JPEG/PNG/WebP allowed")
    os.makedirs(AVATAR_DIR, exist_ok=True)
    ext = file.filename.rsplit(".", 1)[-1] if "." in file.filename else "jpg"
    fname = f"{user.id}_{uuid.uuid4().hex[:8]}.{ext}"
    with open(os.path.join(AVATAR_DIR, fname), "wb") as f:
        shutil.copyfileobj(file.file, f)
    user.avatar = f"/uploads/{fname}"
    db.commit()
    return {"avatarUrl": user.avatar}

@router.get("/search")
def search_users(q: str = "", me=Depends(get_current_user), db: Session = Depends(get_db)):
    if not q.strip():
        return []
    pattern = f"%{q}%"
    users = (
        db.query(User)
        .filter(
            User.id != me.id,
            (User.login.ilike(pattern) | User.display_name.ilike(pattern)),
        )
        .limit(10)
        .all()
    )
    result = []
    for u in users:
        friendship = db.query(Friendship).filter(
            or_(
                and_(Friendship.requester_id == me.id, Friendship.addressee_id == u.id),
                and_(Friendship.requester_id == u.id, Friendship.addressee_id == me.id),
            )
        ).first()
        if friendship and friendship.status == "accepted":
            status = "friend"
        elif friendship and friendship.status == "pending":
            status = "pending"
        else:
            status = "none"
        result.append({
            "id": u.id, "login": u.login,
            "displayName": u.display_name, "avatar": u.avatar,
            "friendshipStatus": status,
        })
    return result


@router.get("/by-id/{user_id}")
def get_user_by_id(user_id: int, db: Session = Depends(get_db), _=Depends(get_current_user)):
    user = db.query(User).filter(User.id == user_id).first()
    if not user:
        raise HTTPException(404, "User not found")
    return user_out(user)

@router.get("/{login}")
def get_user(login: str, db: Session = Depends(get_db), _=Depends(get_current_user)):
    user = db.query(User).filter(User.login == login).first()
    if not user:
        raise HTTPException(404, "User not found")
    return user_out(user)

@router.delete("/me")
def delete_me(user=Depends(get_current_user), db: Session = Depends(get_db)):
    from models.workspace_member import WorkspaceMember
    from models.workspace_notification import WorkspaceNotification
    from models.workspace import Workspace

    login = user.login

    memberships = db.query(WorkspaceMember).filter(
        WorkspaceMember.user_id == user.id
    ).all()

    for m in memberships:
        ws = db.query(Workspace).filter(Workspace.id == m.workspace_id).first()
        if not ws:
            continue

        if m.role == "owner":
            # Notify remaining members before deleting workspace
            others = db.query(WorkspaceMember).filter(
                WorkspaceMember.workspace_id == ws.id,
                WorkspaceMember.user_id != user.id,
            ).all()
            for o in others:
                db.add(WorkspaceNotification(
                    user_id=o.user_id,
                    workspace_id=ws.id,
                    type="workspace_deleted",
                    message=f'Workspace "{ws.name}" was deleted because its owner ({login}) deleted their account',
                ))
            db.delete(ws)
        else:
            # Notify other members that this user has left
            others = db.query(WorkspaceMember).filter(
                WorkspaceMember.workspace_id == ws.id,
                WorkspaceMember.user_id != user.id,
            ).all()
            for o in others:
                db.add(WorkspaceNotification(
                    user_id=o.user_id,
                    workspace_id=ws.id,
                    type="member_left",
                    message=f'{login} left "{ws.name}" (account deleted)',
                ))

    db.flush()
    user.is_deleted   = True
    user.login        = f"deleted_{user.id}_{user.login}"
    user.email        = None
    user.password     = None
    user.totp_secret  = None
    user.totp_enabled = False
    db.commit()
    return {"ok": True}

class TotpVerifyBody(BaseModel):
    code: str

@router.post("/2fa/setup")
def setup_2fa(user=Depends(get_current_user), db: Session = Depends(get_db)):
    secret = pyotp.random_base32()
    user.totp_secret = secret
    db.commit()
    uri = pyotp.totp.TOTP(secret).provisioning_uri(
        user.email or user.login, issuer_name="ft_transcendence"
    )
    img = qrcode.make(uri)
    buf = io.BytesIO()
    img.save(buf, format="PNG")
    qr_b64 = base64.b64encode(buf.getvalue()).decode()
    return {"secret": secret, "qr_code": f"data:image/png;base64,{qr_b64}"}

@router.post("/2fa/verify")
def verify_2fa(body: TotpVerifyBody, user=Depends(get_current_user), db: Session = Depends(get_db)):
    if not user.totp_secret:
        raise HTTPException(400, "2FA not set up")
    if not pyotp.TOTP(user.totp_secret).verify(body.code):
        raise HTTPException(403, "Invalid code")
    user.totp_enabled = True
    db.commit()
    return {"ok": True}

@router.post("/2fa/disable")
def disable_2fa(user=Depends(get_current_user), db: Session = Depends(get_db)):
    user.totp_enabled = False
    user.totp_secret = None
    db.commit()
    return {"ok": True}
