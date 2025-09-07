import pika
from pika.exceptions import AMQPConnectionError


def start():
    connection = pika.BlockingConnection(
        pika.ConnectionParameters(host="localhost", port=5672)
    )

    channel = connection.channel()
    channel.queue_declare(queue="test_queue")

    channel.basic_publish(
        exchange="",
        routing_key="test_queue",
        body=bytes("Hello Joe!", "utf-8"),
    )

    print("[x] Sent 'Hello Joe!'")

    connection.close()

if __name__ == "__main__":
    print("Client running...")
    try:
        start()
    except AMQPConnectionError as err:
        print("Connection error!")
