![ForestMQ](assets/fmq_logo.png?raw=true "ForestMQ")
Message Queue built on [libevent](https://libevent.org/)


⚠️ Moved to 👉 [ForestMQ](https://github.com/josefdigital/forestmq)

### Project Status
🎉 Forest MQ is now usable for test / hobby projects or the curious.

### Features
- TCP ✅
- ampq-v1.0 `TODO`
- Multiple queues `TODO`
- Message acknowledgement `TODO`
- Message durability / persist `TODO`
- exchange types (e.g. fan) `TODO`
- Logging ✅
- Docker ✅
- Daemon ✅
- Topics `TODO`
- MacOS distribution with brew `TODO`*
- Linux distribution with snap `TODO`*
- Message encryption `TODO`

\* *ForestMQ currently only support UNIX like systems.*
### Quick Start
#### Run with Docker
```
docker run -p 8005:8005 bandnoticeboard/forestmq:0.6.0
```

Forest MQ is still in very early stages of development, but
it is usable for test projects. 

As it's missing some fundamental networking, security & stability requirements,
it's not recommended for production environments.

To test Forest MQ end to end you can run the following 2 Python scripts:

- `examples/python/provider.py`
- `examples/python/consumer.py`

This will demonstrate that the message queue excepts messages 
enqueued over HTTP to the queue & the consumer script will
dequeue messages off the queue.


### CLI options
| Arg           | Info                                        | Example             |
|---------------|---------------------------------------------|---------------------|
| `--msg-size`  | integer number of bytes e.g                 | `--msg-size 10000`  |
| `--port`      | integer e.g                                 | `--port 3000`       |
| `--log-level` | Default is `none` (No logging)              | `--log-level none`  |    
|               | Debugging level logging (includes all logs) | `--log-level debug` |
| `--daemon`    | Run ForestMQ in daemon mode                 | `--daemon`          |
| `--hosts`     | Add a list of allowed hosts                 | `--hosts localhost,127.0.0.1` |
### Options
#### Provider Options:
- `message` - A string, bytes or JSON object.
- `destroy` - If set to `true` then the queue will be destroyed & no more messages
will be stored on the queue until the `destroy: true` options is removed from the provider's
request body or set to `false`.

### Environment Variables
| Name              | Type | Info                         |
|-------------------|------|------------------------------|
| `FORESTMQ_DAEMON` | INT  | Run in daemon mode.          |
| `FORESTMQ_PORT`   | INT  | TCP Server port to listen on |

### Clients
- ForestMQ Python client `TODO`
- ForestMQ NodeJS client `TODO`
- ForestMQ Go client `TODO`
- ForestMQ Dart client `TODO`

### Health
To check the health of a running ForestMQ instance:
```
curl http://localhost:8005/health
```
ForestMQ will respond with the following JSON response
```
{"queue_empty":true,"queue_length": 0,"status":"OK","request_start":"Sun Jul 28 18:59:44 2024\n","request_end":"Sun Jul 28 18:59:44 2024\n"}
```

### Security
N/A
    

### Support
TODO

### Contributions
TODO

### Contact
TODO

### Acknowledgement
- [libevent](https://libevent.org/)
