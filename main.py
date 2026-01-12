from fastapi import FastAPI

app = FastAPI(title="Forum API")

@app.get("/")
def root():
    return {"status": "API is running"}