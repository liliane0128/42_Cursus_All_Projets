import os, uuid, shutil
from fastapi import APIRouter, Depends, HTTPException, UploadFile, File

from app.core.security import get_current_user
from app.core.config import UPLOAD_DIR

router = APIRouter()

ALLOWED_TYPES = {"image/jpeg", "image/png", "image/webp", "image/gif"}
MAX_SIZE = 5 * 1024 * 1024  # 5 MB


@router.post("/upload")
async def upload_file(file: UploadFile = File(...), _=Depends(get_current_user)):
    if file.content_type not in ALLOWED_TYPES:
        raise HTTPException(400, "Only JPEG/PNG/WebP/GIF images are allowed")

    data = await file.read()
    if len(data) > MAX_SIZE:
        raise HTTPException(413, "File exceeds 5 MB limit")

    ext = file.filename.rsplit(".", 1)[-1] if "." in file.filename else "jpg"
    fname = f"{uuid.uuid4().hex}.{ext}"

    os.makedirs(UPLOAD_DIR, exist_ok=True)
    with open(os.path.join(UPLOAD_DIR, fname), "wb") as f:
        f.write(data)

    return {"url": f"/uploads/{fname}"}
