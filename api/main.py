from fastapi import FastAPI, Depends, HTTPException
from sqlalchemy.orm import Session
from datetime import datetime

from database import engine, get_db
from models import Base, Thread, Post
from pydantic import BaseModel

# Create FastAPI app
app = FastAPI(title="Forum API")

# Create database tables on startup
Base.metadata.create_all(bind=engine)

class ThreadCreate(BaseModel):
    title: str
    summary: str

@app.get("/")
def health_check():
    return {"status": "ok"}


# THREADS
@app.get("/threads")
def get_threads(db: Session = Depends(get_db)):
    threads = db.query(Thread).all()
    return threads


@app.post("/threads")
def create_thread(data: ThreadCreate, db: Session = Depends(get_db)):
    thread = Thread(
        title=data.title,
        summary=data.summary,
        time=datetime.utcnow().isoformat()
    )
    db.add(thread)
    db.commit()
    db.refresh(thread)
    return thread


# POSTS (INSIDE THREADS)
@app.get("/threads/{thread_id}/posts")
def get_thread_posts(thread_id: int, db: Session = Depends(get_db)):
    thread = db.query(Thread).filter(Thread.id == thread_id).first()
    if not thread:
        raise HTTPException(status_code=404, detail="Thread not found")

    posts = (
        db.query(Post)
        .filter(Post.thread_id == thread_id)
        .order_by(Post.id.asc())
        .all()
    )
    return posts


@app.post("/threads/{thread_id}/posts")
def create_post(thread_id: int, comment: str, db: Session = Depends(get_db)):
    thread = db.query(Thread).filter(Thread.id == thread_id).first()
    if not thread:
        raise HTTPException(status_code=404, detail="Thread not found")

    post = Post(
        comment=comment,
        thread_id=thread_id,
        time=datetime.utcnow().isoformat()
    )
    db.add(post)
    db.commit()
    db.refresh(post)
    return post
