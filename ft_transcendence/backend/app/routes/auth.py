import re
import httpx
import pyotp
import smtplib
import os
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from fastapi import APIRouter, HTTPException, Depends
from fastapi.responses import RedirectResponse, JSONResponse
from pydantic import BaseModel
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import hash_password, verify_password, create_token, get_current_user
from app.core.config import FT_CLIENT_ID, FT_CLIENT_SECRET, FT_REDIRECT_URI
from app.core.config import GITHUB_CLIENT_ID, GITHUB_CLIENT_SECRET, GITHUB_REDIRECT_URI, FRONTEND_URL
from models.user import User

router = APIRouter()

GMAIL_USER     = os.getenv("GMAIL_USER", "")
GMAIL_PASSWORD = os.getenv("GMAIL_PASSWORD", "")

class RegisterBody(BaseModel):
    login: str
    email: str
    password: str

class LoginBody(BaseModel):
    login: str
    password: str

def user_out(u: User):
    return {
        "id": u.id, "login": u.login,
        "displayName": u.display_name, "email": u.email,
        "avatar": u.avatar, "bio": u.bio or "",
        "status": u.status or "offline",
    }

def send_gdpr_email(to_email: str, subject: str, body_html: str):
    if not GMAIL_USER or not GMAIL_PASSWORD:
        print("[EMAIL] No Gmail credentials configured, skipping email")
        return
    msg = MIMEMultipart("alternative")
    msg["Subject"] = subject
    msg["From"]    = f"Collab42 <{GMAIL_USER}>"
    msg["To"]      = to_email
    msg.attach(MIMEText(body_html, "html"))
    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        server.login(GMAIL_USER, GMAIL_PASSWORD)
        server.sendmail(GMAIL_USER, to_email, msg.as_string())

@router.post("/register")
def register(body: RegisterBody, db: Session = Depends(get_db)):
    if len(body.password) < 8:
        raise HTTPException(400, "Password must be at least 8 characters")
    if not body.email or not body.email.strip():
        raise HTTPException(400, "Email is required")
    if not re.match(r"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$", body.email):
        raise HTTPException(400, "Invalid email format")
    if db.query(User).filter((User.login == body.login) | (User.email == body.email)).first():
        raise HTTPException(400, "Login or email already taken")
    user = User(login=body.login, email=body.email,
                display_name=body.login, password=hash_password(body.password))
    db.add(user)
    db.commit()
    db.refresh(user)
    return {"token": create_token(user.id), "user": user_out(user)}

@router.post("/login")
def login(body: LoginBody, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.login == body.login).first()
    if not user or not user.password or not verify_password(body.password, user.password):
        raise HTTPException(401, "Invalid credentials")
    if user.totp_enabled:
        return {"requires_2fa": True, "user_id": user.id}
    user.status = "online"
    db.commit()
    return {"token": create_token(user.id), "user": user_out(user)}

class TwoFALoginBody(BaseModel):
    user_id: int
    code: str

@router.post("/2fa/login")
def login_2fa(body: TwoFALoginBody, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.id == body.user_id).first()
    if not user or not user.totp_enabled or not user.totp_secret:
        raise HTTPException(400, "2FA not enabled for this user")
    if not pyotp.TOTP(user.totp_secret).verify(body.code):
        raise HTTPException(403, "Invalid 2FA code")
    user.status = "online"
    db.commit()
    return {"token": create_token(user.id), "user": user_out(user)}

@router.get("/me")
def me(user=Depends(get_current_user)):
    return user_out(user)

# ── GDPR ──────────────────────────────────────────────────────────────────────

@router.get("/me/export")
def export_my_data(user: User = Depends(get_current_user), db: Session = Depends(get_db)):
    from models.task import Task
    from models.workspace_member import WorkspaceMember
    from models.workspace import Workspace

    memberships = db.query(WorkspaceMember).filter(
        WorkspaceMember.user_id == user.id
    ).all()
    ws_ids = [m.workspace_id for m in memberships]

    workspaces = db.query(Workspace).filter(Workspace.id.in_(ws_ids)).all()

    tasks = (
        db.query(Task)
        .filter(Task.workspace_id.in_(ws_ids))
        .all()
    )

    data = {
        "account": {
            "id": user.id,
            "login": user.login,
            "display_name": user.display_name,
            "email": user.email,
            "bio": user.bio or "",
            "status": user.status,
            "avatar": user.avatar,
            "two_factor_enabled": user.totp_enabled,
        },
        "workspaces": [
            {
                "id": w.id,
                "name": w.name,
                "description": w.description,
                "color": w.color,
                "deadline": w.deadline,
                #"created_at": w.created_at,
            }
            for w in workspaces
        ],
        "tasks": [
            {
                "id": t.id,
                "title": t.title,
                "status": t.status,
                "priority": t.priority,
                "due_date": t.due_date,
                "workspace_id": t.workspace_id,
                #"created_at": t.created_at,
            }
            for t in tasks
        ],
    }

    if user.email:
        try:
            send_gdpr_email(
                user.email,
                "Your data export — Collab42",
                f"""
                <body style="font-family:sans-serif;background:#f5f5f3;padding:40px 20px;">
                <div style="max-width:480px;margin:0 auto;background:#fff;border-radius:16px;padding:40px;">
                    <h2 style="color:#111827;">Your data export</h2>
                    <p style="color:#6b7280;">Hi {user.display_name}, your data export was requested and downloaded successfully.</p>
                    <p style="color:#6b7280;">If you didn't request this, please contact us immediately.</p>
                    <p style="color:#9ca3af;font-size:12px;">© 2026 Collab42 · 42 Paris</p>
                </div>
                </body>
                """
            )
        except Exception as e:
            print(f"[EMAIL ERROR] {e}")

    return JSONResponse(content=data)


@router.delete("/me")
def delete_my_account(user: User = Depends(get_current_user), db: Session = Depends(get_db)):
    from models.task import Task
    from models.workspace_member import WorkspaceMember
    from models.workspace import Workspace

    email = user.email
    name  = user.display_name

    memberships = db.query(WorkspaceMember).filter(
        WorkspaceMember.user_id == user.id
    ).all()

    for membership in memberships:
        ws_id = membership.workspace_id

        all_members = db.query(WorkspaceMember).filter(
            WorkspaceMember.workspace_id == ws_id
        ).all()

        if len(all_members) == 1:
            # Only member → delete entire workspace (tasks deleted via CASCADE)
            ws = db.query(Workspace).filter(Workspace.id == ws_id).first()
            if ws:
                db.delete(ws)
        else:
            # Multiple members → transfer ownership if owner, then remove membership
            if membership.role == "owner":
                next_owner = (
                    db.query(WorkspaceMember)
                    .filter(
                        WorkspaceMember.workspace_id == ws_id,
                        WorkspaceMember.user_id != user.id
                    )
                    .order_by(WorkspaceMember.id.asc())
                    .first()
                )
                if next_owner:
                    next_owner.role = "owner"

            db.delete(membership)

    user.is_deleted   = True
    user.login        = f"deleted_{user.id}_{user.login}"
    user.email        = None
    user.password     = None
    user.totp_secret  = None
    user.totp_enabled = False
    db.commit()

    if email:
        try:
            send_gdpr_email(
                email,
                "Your account has been deleted — Collab42",
                f"""
                <body style="font-family:sans-serif;background:#f5f5f3;padding:40px 20px;">
                <div style="max-width:480px;margin:0 auto;background:#fff;border-radius:16px;padding:40px;">
                    <h2 style="color:#111827;">Account deleted</h2>
                    <p style="color:#6b7280;">Hi {name}, your Collab42 account and all associated data have been permanently deleted.</p>
                    <p style="color:#6b7280;">If you didn't request this, please contact us immediately.</p>
                    <p style="color:#9ca3af;font-size:12px;">© 2026 Collab42 · 42 Paris</p>
                </div>
                </body>
                """
            )
        except Exception as e:
            print(f"[EMAIL ERROR] {e}")

    return {"ok": True}

# ── OAuth ─────────────────────────────────────────────────────────────────────

@router.get("/42")
def oauth_42():
    return RedirectResponse(
        f"https://api.intra.42.fr/oauth/authorize?client_id={FT_CLIENT_ID}"
        f"&redirect_uri={FT_REDIRECT_URI}&response_type=code&scope=public"
    )

@router.get("/callback/42")
async def callback_42(code: str, db: Session = Depends(get_db)):
    async with httpx.AsyncClient(timeout=30.0) as client:
        r = await client.post("https://api.intra.42.fr/oauth/token", data={
            "grant_type": "authorization_code", "client_id": FT_CLIENT_ID,
            "client_secret": FT_CLIENT_SECRET, "code": code, "redirect_uri": FT_REDIRECT_URI,
        })
        if r.status_code != 200:
            return RedirectResponse(f"{FRONTEND_URL}/login?error=oauth_failed")
        ft_user = (await client.get("https://api.intra.42.fr/v2/me",
            headers={"Authorization": f"Bearer {r.json()['access_token']}"})).json()
    ft_id = ft_user["id"]
    user = db.query(User).filter(User.ft_id == ft_id).first()
    if not user:
        user = User(
            login=ft_user["login"], email=ft_user.get("email", ""),
            display_name=ft_user.get("displayname", ft_user["login"]),
            avatar=ft_user.get("image", {}).get("link"), ft_id=ft_id
        )
        db.add(user)
        db.commit()
        db.refresh(user)
    if user.totp_enabled:
        return RedirectResponse(f"{FRONTEND_URL}/login?requires_2fa=true&user_id={user.id}")
    user.status = "online"
    db.commit()
    return RedirectResponse(f"{FRONTEND_URL}/dashboard?token={create_token(user.id)}")

@router.get("/github")
def oauth_github():
    return RedirectResponse(
        f"https://github.com/login/oauth/authorize?client_id={GITHUB_CLIENT_ID}"
        f"&redirect_uri={GITHUB_REDIRECT_URI}&scope=user:email"
    )

@router.get("/callback/github")
async def callback_github(code: str, db: Session = Depends(get_db)):
    async with httpx.AsyncClient() as client:
        r = await client.post("https://github.com/login/oauth/access_token",
            json={"client_id": GITHUB_CLIENT_ID, "client_secret": GITHUB_CLIENT_SECRET,
                  "code": code, "redirect_uri": GITHUB_REDIRECT_URI},
            headers={"Accept": "application/json"})
        token = r.json().get("access_token")
        if not token:
            return RedirectResponse(f"{FRONTEND_URL}/login?error=oauth_failed")
        gh = (await client.get("https://api.github.com/user",
            headers={"Authorization": f"Bearer {token}"})).json()
    gh_id = gh["id"]
    user = db.query(User).filter(User.github_id == gh_id).first()
    if not user:
        user = User(
            login=gh["login"], email=gh.get("email", ""),
            display_name=gh.get("name") or gh["login"],
            avatar=gh.get("avatar_url"), github_id=gh_id
        )
        db.add(user)
        db.commit()
        db.refresh(user)
    if user.totp_enabled:
        return RedirectResponse(f"{FRONTEND_URL}/login?requires_2fa=true&user_id={user.id}")
    user.status = "online"
    db.commit()
    return RedirectResponse(f"{FRONTEND_URL}/dashboard?token={create_token(user.id)}")
