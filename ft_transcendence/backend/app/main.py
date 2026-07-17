import time
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import Response
from fastapi.staticfiles import StaticFiles
from prometheus_client import Counter, Histogram, generate_latest, CONTENT_TYPE_LATEST

REQUEST_COUNT = Counter('http_requests_total', 'Total HTTP requests', ['method', 'endpoint', 'status'])
REQUEST_LATENCY = Histogram('http_request_duration_seconds', 'HTTP request latency')

from app.core.database import init_db
from app.core.config import UPLOAD_DIR
from app.routes.auth import router as auth_router
from app.routes.users import router as users_router
from app.routes.ai import router as ai_router
from app.routes.files import router as files_router
from app.routes.messages import router as messages_router
from app.routes.workspace import router as workspace_router
from app.routes.task import router as task_router
from app.routes.password_reset import router as password_reset_router
from app.routes.links import router as links_router
from app.routes.invites import router as invites_router
from app.routes.notifications import router as notifications_router
from app.routes.channel import router as channel_router
from app.routes.friendship import router as friendship_router
from app.routes.dm import router as dm_router

app = FastAPI(title="ft_transcendence API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173", "http://localhost"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.middleware("http")
async def metrics_middleware(request: Request, call_next):
    start = time.time()
    response = await call_next(request)
    duration = time.time() - start
    REQUEST_COUNT.labels(method=request.method, endpoint=request.url.path, status=response.status_code).inc()
    REQUEST_LATENCY.observe(duration)
    return response

@app.on_event("startup")
async def startup():
    init_db()
    from models.reset_token import ResetToken
    from models.workspace_member import WorkspaceMember  # noqa: F401
    from models.workspace_invite import WorkspaceInvite  # noqa: F401
    from models.workspace_notification import WorkspaceNotification  # noqa: F401
    from models.workspace import WorkspaceFile  # noqa: F401
    from models.friendship import Friendship  # noqa: F401
    from models.dm_channel import DmChannel  # noqa: F401
    from models.moderation import ModerationRecord  # noqa: F401
    from app.core.database import Base, engine

    Base.metadata.create_all(bind=engine)

    # ── column migrations (safe: no-op if column already exists) ────────────
    from sqlalchemy import text
    migrations = [
        "ALTER TABLE workspaces ADD COLUMN IF NOT EXISTS deadline TEXT",
        "ALTER TABLE workspaces ADD COLUMN IF NOT EXISTS is_completed BOOLEAN DEFAULT FALSE",
        "ALTER TABLE users ADD COLUMN IF NOT EXISTS is_deleted BOOLEAN DEFAULT FALSE",
    ]
    with engine.connect() as conn:
        for sql in migrations:
            conn.execute(text(sql))
        conn.commit()

app.include_router(auth_router,  prefix="/api/auth",  tags=["auth"])
app.include_router(users_router, prefix="/api/users", tags=["users"])
app.include_router(ai_router,    prefix="/api/ai",    tags=["ai"])
app.include_router(files_router,    prefix="/api/files",    tags=["files"])
app.include_router(messages_router, prefix="/api/messages", tags=["messages"])
app.include_router(workspace_router, prefix="/api/workspaces", tags=["workspaces"])
app.include_router(task_router,      prefix="/api/tasks",      tags=["tasks"])
app.include_router(password_reset_router, prefix="/api", tags=["auth"])
app.include_router(links_router, prefix="/api/links", tags=["links"])
app.include_router(invites_router, prefix="/api/invites", tags=["invites"])
app.include_router(notifications_router, prefix="/api/notifications", tags=["notifications"])
app.include_router(channel_router,       prefix="/api/channels",      tags=["channels"])
app.include_router(friendship_router,    prefix="/api/friends",       tags=["friends"])
app.include_router(dm_router,            prefix="/api/dm",            tags=["dm"])

from app.core.config import WORKSPACE_FILES_DIR
import os
os.makedirs(UPLOAD_DIR, exist_ok=True)
os.makedirs(WORKSPACE_FILES_DIR, exist_ok=True)

app.mount("/uploads", StaticFiles(directory=UPLOAD_DIR), name="uploads")
app.mount("/avatars", StaticFiles(directory=UPLOAD_DIR), name="avatars")

@app.get("/metrics")
def metrics():
    return Response(generate_latest(), media_type=CONTENT_TYPE_LATEST)

@app.get("/api/health")
def health():
    return {"status": "ok"}

@app.get("/")
def root():
    return {"message": "Hello, ft_transcendence!"}