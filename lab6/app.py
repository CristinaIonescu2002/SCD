import time

import redis
from flask import Flask
import socket
import os

app = Flask(__name__)
cache = redis.Redis(host='redis', port=6379)

# redis_host = "redis"
# redis_port = 6379
# redis_client = redis.Redis(host=redis_host, port=redis_port)

redis_host = os.getenv("REDIS_HOST", "redis")
redis_port = os.getenv("REDIS_PORT", 6379)
redis_password = os.getenv("REDIS_PASSWORD", "your_redis_password")
rate_limit_window = int(os.getenv("RATE_LIMIT_WINDOW", 10))

redis_client = redis.StrictRedis(
    host=redis_host,
    port=redis_port,
    password=redis_password,
    decode_responses=True
)

cache = redis.StrictRedis(
    host=redis_host,
    port=redis_port,
    password=redis_password,
    decode_responses=True
)

def get_hit_count():
    retries = 5
    while True:
        try:
            return cache.incr('hits')
        except redis.exceptions.ConnectionError as exc:
            if retries == 0:
                raise exc
            retries -= 1
            time.sleep(0.5)

@app.route('/')
def hello():
    hostname = socket.gethostname()

    current_time = time.time()

    redis_client.zadd("access_times", {current_time: current_time})
    ten_seconds_ago = current_time - 10
    redis_client.zremrangebyscore("access_times", 0, ten_seconds_ago)
    count10Sec = redis_client.zcard("access_times")

    count = get_hit_count()


    return f'Hello World! I have been seen {count} times. The host name id {hostname} and accesed {count10Sec} resoureces in 10 seconds\n'