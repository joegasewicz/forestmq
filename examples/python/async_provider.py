import asyncio
from forestmq import ForestMQ

async def async_example():
    fmq = ForestMQ(domain="http://localhost:8005")
    result = fmq.provider.send_msg_sync({
        "id": 1,
        "message": "Hello!",
    })
    print(f"Result: {result}")


if __name__ == "__main__":
    asyncio.run(async_example())
