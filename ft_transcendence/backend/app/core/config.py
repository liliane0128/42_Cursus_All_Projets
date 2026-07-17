import os
from dotenv import load_dotenv
load_dotenv()

JWT_SECRET      = os.getenv("JWT_SECRET", "change_me")
JWT_ALGORITHM   = "HS256"
JWT_EXPIRE_HRS  = int(os.getenv("JWT_EXPIRE_HOURS", "24"))
FT_CLIENT_ID    = os.getenv("FT_CLIENT_ID", "")
FT_CLIENT_SECRET= os.getenv("FT_CLIENT_SECRET", "")
FT_REDIRECT_URI = os.getenv("FT_REDIRECT_URI", "")
GITHUB_CLIENT_ID    = os.getenv("GITHUB_CLIENT_ID", "")
GITHUB_CLIENT_SECRET= os.getenv("GITHUB_CLIENT_SECRET", "")
GITHUB_REDIRECT_URI = os.getenv("GITHUB_REDIRECT_URI", "")
FRONTEND_URL    = os.getenv("FRONTEND_URL", "http://localhost:5173")
DB_PATH         = os.getenv("DB_PATH", "data/app.db")
UPLOAD_DIR           = os.getenv("UPLOAD_DIR", "data/uploads")
AVATAR_DIR           = UPLOAD_DIR
WORKSPACE_FILES_DIR  = os.getenv("WORKSPACE_FILES_DIR", "data/workspace_files")
