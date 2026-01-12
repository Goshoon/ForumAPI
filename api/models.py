from sqlalchemy import Column, Integer, Text, ForeignKey
from sqlalchemy.orm import relationship
from database import Base

class Thread(Base):
    __tablename__ = "threads"

    id = Column("ID", Integer, primary_key=True, index=True)
    summary = Column("SUMMARY", Text, nullable=False)
    time = Column("TIME", Text, nullable=False)

    posts = relationship("Post", back_populates="thread_obj", cascade="all, delete")


class Post(Base):
    __tablename__ = "posts"

    id = Column("ID", Integer, primary_key=True, index=True)
    comment = Column("COMMENT", Text, nullable=False)
    time = Column("TIME", Text, nullable=False)

    thread = Column(
        "THREAD",
        Integer,
        ForeignKey("threads.ID"),
        nullable=False
    )

    thread_obj = relationship("Thread", back_populates="posts")
