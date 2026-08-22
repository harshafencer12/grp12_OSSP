# Practical-05: Inter-Process Communication Using Pipes

## Objective

1. Implement a producer-consumer communication system using anonymous pipes where the parent process generates data and the child process consumes it. Measure the communication efficiency.

2. Develop a program that executes the equivalent of the shell command `ls -l | grep ".c"` using `fork()`, `pipe()`, `dup2()`, and `exec()` system calls.

---

# Part 1: Producer-Consumer Using Anonymous Pipe

## Description

An anonymous pipe is used for communication between a parent and child process.

The parent process acts as the producer and generates integer data.

The child process acts as the consumer and reads the data from the pipe.

The pipe provides one-way communication between the processes.

## System Calls Used

- `pipe()` - Creates the communication channel.
- `fork()` - Creates the child process.
- `write()` - Producer writes data into the pipe.
- `read()` - Consumer reads data from the pipe.
- `close()` - Closes unused pipe ends.
- `wait()` - Parent waits for the child process.

## Compilation

```bash
gcc producer_consumer.c -o producer_consumer
