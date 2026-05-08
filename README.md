# Distributed Log Streamer (Kafka-lite)

A lightweight, high-performance message streaming system built in C++ with TCP networking, multithreading, and real-time broadcasting capabilities.

## Overview

This project implements a distributed publish-subscribe messaging system similar to Apache Kafka or RabbitMQ, but simplified and built from scratch in C++. It demonstrates core concepts in:

- **TCP/IP networking** with socket programming
- **Multithreading** and concurrent client handling
- **Producer-consumer patterns**
- **Message broadcasting** in real-time
- **Thread-safe data structures** with mutexes

## Architecture

```
┌──────────┐                  ┌──────────┐
│Producer A│─────┐            │Consumer X│
└──────────┘     │            └──────────┘
                 │                  ▲
┌──────────┐     │                  │
│Producer B│─────┼──────► ┌─────────┴────────┐
└──────────┘     │        │   BROKER SERVER  │
                 │        │  (routes msgs)   │
┌──────────┐     │        └─────────┬────────┘
│Producer C│─────┘                  │
└──────────┘                        ▼
                            ┌──────────┐
                            │Consumer Y│
                            └──────────┘
```

### Components

1. **Broker Server** - Multi-threaded TCP server that accepts connections and broadcasts messages
2. **Producer Client** - Sends messages to the broker
3. **Consumer Client** - Receives messages from the broker in real-time

## Features

### Current Implementation (Phase 1)

- ✅ **TCP Server/Client Architecture** - Low-level socket programming
- ✅ **Multi-threaded Broker** - Handles multiple clients simultaneously
- ✅ **Real-time Broadcasting** - Messages sent to all connected consumers
- ✅ **Thread-safe Operations** - Mutex-protected client list
- ✅ **Persistent Connections** - Consumers stay connected and listen continuously

### Planned Features (Future Phases)

- **Topic-based Routing** - Subscribe to specific channels (e.g., "payments", "logs")
- ⏳ **Message Persistence** - Write messages to disk for replay
- ⏳ **Message Offsets** - Consumers can request historical messages
- ⏳ **Protocol Buffers** - Structured message format with headers
- ⏳ **Consumer Groups** - Load balancing across multiple consumers
- ⏳ **Acknowledgments** - Confirm message delivery

## 📁 Project Structure

```
C-plus-plus-Work/
├── README.md
├── .gitignore
├── broker/
│   └── server.cpp          # Multi-threaded broker server
├── producer/
│   └── producer.cpp        # Producer client
├── consumer/
│   └── consumer.cpp        # Consumer client
└── common/
    └── message.h           # (Future) Shared message protocol
```

## Build & Run

### Prerequisites

- C++17 or later
- g++ compiler
- macOS or Linux

### Compilation

```bash
cd broker
g++ -std=c++17 -pthread server.cpp -o brokerServer

cd ../producer
g++ -std=c++17 producer.cpp -o producerClient

cd ../consumer
g++ -std=c++17 consumer.cpp -o consumerClient
```

### Running the System

**Terminal 1: Start the broker**
```bash
cd broker
./brokerServer
```

**Terminal 2: Start a consumer**
```bash
cd consumer
./consumerClient
```

**Terminal 3: Send messages from producer**
```bash
cd producer
./producerClient "Hello World!"
./producerClient "This is a test message"
```

The consumer(s) will receive messages in real-time!

## Testing

### Test Multiple Consumers

1. Start the broker
2. Open 2-3 consumer terminals
3. Send messages from producer
4. **All consumers receive the same messages** (broadcast behavior)

### Example Output

**Broker:**
```
Broker listening on port 9092...
Client connected (fd: 4)
Client connected (fd: 5)
Client connected (fd: 6)
Received from client 6: Hello World!
Client disconnected (fd: 6)
```

**Consumer:**
```
Connected to broker!
Waiting for messages... (Press Ctrl+C to exit)

📩 Received: Hello World!
📩 Received: This is a test message
```

## 💡 Technical Highlights

### Networking
- Low-level POSIX socket API (`socket`, `bind`, `listen`, `accept`)
- TCP stream-based communication
- Non-blocking I/O with concurrent clients

### Concurrency
- **One thread per client** - Each connection handled independently
- **Mutex-protected shared state** - Thread-safe client list
- **Detached threads** - Fire-and-forget client handling

### Performance Considerations
- Efficient message broadcasting to multiple clients
- Minimal memory copying
- Scalable to dozens of concurrent connections

## 🎓 Learning Outcomes

This project demonstrates proficiency in:

- **Systems Programming** - Low-level C++ and POSIX APIs
- **Network Programming** - TCP/IP, sockets, client-server architecture
- **Concurrent Programming** - Threads, mutexes, race conditions
- **Software Architecture** - Designing distributed systems
- **Infrastructure Engineering** - Building message brokers and queues

## Future Enhancements

### Phase 2: Topics & Subscriptions
- Add topic-based routing (like Kafka topics)
- Consumers subscribe to specific channels
- Producers publish to specific topics

### Phase 3: Persistence
- Write messages to disk (append-only log)
- Allow consumers to replay historical messages
- Implement message offsets

### Phase 4: Advanced Features
- Consumer groups for load balancing
- Message acknowledgments & retries
- Compression and batching
- Monitoring dashboard (web UI)

## 📚 Use Cases

This architecture is similar to production systems used for:

- **Event Streaming** - Real-time data pipelines
- **Log Aggregation** - Collecting logs from multiple services
- **Microservices Communication** - Decoupled service-to-service messaging
- **Real-time Analytics** - Processing streams of events


## 🔗 Related Technologies

- **Apache Kafka** - Distributed event streaming platform
- **RabbitMQ** - Message broker with AMQP protocol
- **Redis Pub/Sub** - In-memory message broker
- **NATS** - Lightweight messaging system

---

**Built with:** C++17, POSIX Sockets, Multithreading  
