import asyncio
from typing import Union

from codecs import StreamReader, StreamWriter


class Client:

    reader: StreamReader
    writer: StreamWriter

    def __init__(
            self,
            host="127.0.0.1",
            port=4379,
    ):
        self.host = host
        self.port = port

    async def connect(self) -> None:
        self.reader, self.writer = await asyncio.open_connection(
            self.host,
            self.port,
        )

    async def send(self, data: bytes) -> None:
        self.writer.write(data)
        await self.writer.drain()

    async def receive(self, n: int):
        return await self.reader.readexactly(n)

    async def close(self):
        self.writer.close()
        await self.writer.wait_closed()
