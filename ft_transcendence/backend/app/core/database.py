import os
from sqlalchemy import create_engine
from sqlalchemy.orm import declarative_base, sessionmaker

DATABASE_URL = os.getenv("DATABASE_URL", "postgresql://postgres:postgres@db:5432/transcendence")

engine       = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base         = declarative_base()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

def init_db():
    import models.user
    import models.message
    import models.workspace
    import models.task
    import models.link
    Base.metadata.create_all(bind=engine)


def init_reset_tokens():
    from models.reset_token import ResetToken
    from app.core.database import Base, engine
    Base.metadata.create_all(bind=engine)
