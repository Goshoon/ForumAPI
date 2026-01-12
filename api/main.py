from fastapi import FastAPI, Depends
from sqlalchemy.orm import Session
from datetime import datetime

from database import engine, get_db
from models import Base, Post

# Create FastAPI app
app = FastAPI(title="Forum API")

# Create database tables on startup
Base.metadata.create_all(bind=engine)


@app.get("/")
def health_check():
    return {"status": "ok"}


@app.get("/posts")
def get_posts(db: Session = Depends(get_db)):
    posts = db.query(Post).all()
    return posts


@app.post("/posts")
def create_post(comment: str, db: Session = Depends(get_db)):
    post = Post(
        comment=comment,
        time=datetime.utcnow().isoformat()
    )
    db.add(post)
    db.commit()
    db.refresh(post)
    return post
