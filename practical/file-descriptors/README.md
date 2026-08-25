# File Descriptors

## Objective

To understand Linux file descriptors and demonstrate the use of standard file descriptors, `open()`, `read()`, `write()`, `close()`, `O_CREAT`, `O_TRUNC`, and `dup()`.

## Standard File Descriptors

Every normal process has three standard file descriptors:

| FD | Name | Purpose |
|---|---|---|
| 0 | STDIN_FILENO | Standard Input |
| 1 | STDOUT_FILENO | Standard Output |
| 2 | STDERR_FILENO | Standard Error |

## Programs Included

### 1. stdout.c

Demonstrates writing to standard output using:

```c
write(STDOUT_FILENO, ...);
