import os, secrets, smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from fastapi import APIRouter, HTTPException, Depends
from pydantic import BaseModel
from sqlalchemy.orm import Session
from app.core.database import get_db
from app.core.security import hash_password
from models.user import User
from models.reset_token import ResetToken

router = APIRouter()

FRONTEND_URL   = os.getenv("FRONTEND_URL", "http://localhost:5173")
GMAIL_USER     = os.getenv("GMAIL_USER", "")
GMAIL_PASSWORD = os.getenv("GMAIL_PASSWORD", "")

def send_reset_email(to_email: str, reset_url: str):
    msg = MIMEMultipart("alternative")
    msg["Subject"] = "Reset your password — Collab42"
    msg["From"] = f"Collab42 <{GMAIL_USER}>"
    msg["To"] = to_email
    html = f"""
<!DOCTYPE html>
<html>
<head><meta charset="UTF-8"></head>
<body style="margin:0;padding:0;background:#f5f5f3;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;">
  <table width="100%" cellpadding="0" cellspacing="0" style="padding:40px 20px;">
    <tr><td align="center">
      <table width="480" cellpadding="0" cellspacing="0" style="background:#ffffff;border-radius:16px;overflow:hidden;box-shadow:0 2px 8px rgba(0,0,0,0.08);">
        <!-- Header -->
        <tr>
          <td style="background:#4f46e5;padding:32px 40px;text-align:center;">
            <div style="font-size:28px;font-weight:700;color:#ffffff;letter-spacing:-0.5px;">Collab42</div>
            <div style="font-size:13px;color:#a5b4fc;margin-top:4px;">Team chat &amp; collaboration</div>
          </td>
        </tr>
        <!-- Body -->
        <tr>
          <td style="padding:40px;">
            <h2 style="margin:0 0 12px;font-size:20px;font-weight:600;color:#111827;">Reset your password</h2>
            <p style="margin:0 0 24px;font-size:14px;color:#6b7280;line-height:1.6;">
              We received a request to reset your password. Click the button below to choose a new one. This link expires in 1 hour.
            </p>
            <div style="text-align:center;margin:0 0 24px;">
              <a href="{reset_url}" style="display:inline-block;background:#4f46e5;color:#ffffff;text-decoration:none;font-size:14px;font-weight:600;padding:12px 32px;border-radius:8px;">
                Reset Password
              </a>
            </div>
            <p style="margin:0;font-size:12px;color:#9ca3af;line-height:1.6;">
              If you didn't request this, you can safely ignore this email. Your password will not be changed.
            </p>
          </td>
        </tr>
        <!-- Footer -->
        <tr>
          <td style="padding:20px 40px;border-top:1px solid #f3f4f6;text-align:center;">
            <p style="margin:0;font-size:11px;color:#d1d5db;">© 2026 Collab42 · 42 Paris</p>
            <p style="margin:8px 0 0;font-size:11px;">
              <a href="http://localhost:5173/terms" style="color:#9ca3af;text-decoration:none;margin:0 8px;">Terms of Service</a>
              <span style="color:#e5e7eb;">·</span>
              <a href="http://localhost:5173/privacy" style="color:#9ca3af;text-decoration:none;margin:0 8px;">Privacy Policy</a>
            </p>
          </td>
        </tr>
      </table>
    </td></tr>
  </table>
</body>
</html>
"""
    msg.attach(MIMEText(html, "html"))
    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        server.login(GMAIL_USER, GMAIL_PASSWORD)
        server.sendmail(GMAIL_USER, to_email, msg.as_string())

class ForgotBody(BaseModel):
    email: str

class ResetBody(BaseModel):
    token: str
    password: str

@router.post("/forgot-password")
def forgot_password(body: ForgotBody, db: Session = Depends(get_db)):
    user = db.query(User).filter(User.email == body.email).first()
    if not user:
        return {"ok": True}
    token = secrets.token_urlsafe(32)
    db.add(ResetToken(user_id=user.id, token=token))
    db.commit()
    reset_url = f"{FRONTEND_URL}/reset-password?token={token}"
    try:
        send_reset_email(body.email, reset_url)
    except Exception as e:
        print(f"[EMAIL ERROR] {type(e).__name__}: {e}")
    return {"ok": True}

@router.post("/reset-password")
def reset_password(body: ResetBody, db: Session = Depends(get_db)):
    if len(body.password) < 8:
        raise HTTPException(400, "Password must be at least 8 characters")
    record = db.query(ResetToken).filter(
        ResetToken.token == body.token,
        ResetToken.used == False
    ).first()
    if not record:
        raise HTTPException(400, "Invalid or expired reset link")
    user = db.query(User).filter(User.id == record.user_id).first()
    if not user:
        raise HTTPException(400, "User not found")
    user.password = hash_password(body.password)
    record.used = True
    db.commit()
    return {"ok": True}
