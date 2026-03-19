import asyncio

from forestmq.client import Client
from forestmq.forestmq import ForestMQ


if __name__ == "__main__":
    client = Client()
    fmq = ForestMQ(client)
    asyncio.run(fmq.publish())
