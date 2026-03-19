from forestmq.client import Client

class ForestMQ:

    def __init__(self, client: Client):
        self.client = client

    async def publish(self):
        await self.client.connect()
        frame = bytearray()
        frame.append(0x46)
        frame.append(0x4D)
        frame.append(0x51)
        frame.append(0x50)
        frame.append(0x00)
        frame.append(0x01)

        await self.client.send(frame)
        await self.client.close()
