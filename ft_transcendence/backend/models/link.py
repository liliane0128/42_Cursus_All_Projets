from sqlalchemy import Column, String
from app.core.database import Base

class Link(Base):
    __tablename__ = "links"
    id         = Column(String, primary_key=True)
    chat_id    = Column(String, nullable=False)
    name       = Column(String, nullable=False)
    url        = Column(String, nullable=False)
    created_at = Column(String, nullable=False)
