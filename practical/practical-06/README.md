# Practical 06 - Named Pipes and POSIX Signal Handling

## Objective

1. To create a client-server application using Named Pipes (FIFOs).
2. To study communication between multiple clients and a server using FIFOs.
3. To implement POSIX signal handling for SIGINT, SIGTERM, and SIGUSR1.
4. To demonstrate asynchronous event handling using signal handlers.

---

# Part 1 - Client-Server Communication Using Named Pipes

## Files

- `fifo_server.c` - Server program
- `fifo_client.c` - Client program

## Working

The server creates a common request FIFO:

`/tmp/ossp_request_fifo`

Each client creates a unique response FIFO using its Process ID:

`/tmp/ossp_response_fifo_PID`

Communication flow:

Client -> Request FIFO -> Server -> Client Response FIFO -> Client

The client sends its PID and message to the server. The server processes the message and sends a response to the corresponding client FIFO.

## Compilation

```bash
gcc fifo_server.c -o fifo_server
gcc fifo_client.c -o fifo_client
