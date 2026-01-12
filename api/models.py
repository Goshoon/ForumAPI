from sqlalchemy import Column, Integer, Text
from database import Base

class Post(Base):
    __tablename__ = "posts"

    id = Column("ID", Integer, primary_key=True, index=True)
    comment = Column("COMMENT", Text, nullable=False)
    time = Column("TIME", Text, nullable=False)
