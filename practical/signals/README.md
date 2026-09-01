# Practical - Signals

## Objective

To study Linux signals and understand how processes handle different signals such as SIGINT, SIGCHLD, SIGPIPE, SIGUSR1, SIGTERM, SIGABRT, and SIGSEGV.

---

## Program 1 - SIGINT with Signal Handler

### Description

This program installs a custom signal handler for SIGINT.

SIGINT is normally generated when the user presses Ctrl+C.

The program catches SIGINT and displays:

SIGINT received!

### Compilation

gcc signal1.c -o signal1

### Execution

./signal1

Press Ctrl+C to generate SIGINT.

---

## Program 2 - SIGINT Without Handler

### Description

This program does not define a custom SIGINT handler.

When Ctrl+C is pressed, the default action of SIGINT terminates the process.

### Compilation

gcc signal2.c -o signal2

### Execution

./signal2

Press Ctrl+C.

---

## Program 3 - SIGCHLD

### Description

SIGCHLD is generated for the parent when the state of a child process changes, commonly when the child terminates.

The program creates a child using fork().

The parent waits for the child using wait().

### Compilation

gcc signal3.c -o signal3

### Execution

./signal3

---

## Program 4 - SIGPIPE

### Description

SIGPIPE occurs when a process attempts to write to a pipe after all read ends of the pipe have been closed.

The program creates a pipe and closes the read end before writing.

### Compilation

gcc signal4.c -o signal4

### Execution

./signal4

---

## Program 5 - kill() and SIGUSR1

### Description

The parent creates a child process using fork().

The child installs a SIGUSR1 handler and waits.

The parent uses kill() to send SIGUSR1 to the child.

The parent later sends SIGTERM to terminate the child.

### Compilation

gcc signal5.c -o signal5

### Execution

./signal5

---

## Program 6 - SIGABRT

### Description

This program demonstrates SIGABRT using abort().

The abort() function terminates the process immediately.

Therefore, the statement after abort() is not executed.

### Compilation

gcc signal6.c -o signal6

### Execution

./signal6

---

## Program 7 - SIGSEGV

### Description

This program demonstrates an invalid memory access.

A NULL pointer is dereferenced using:

*p = 10;

This causes a segmentation fault.

### Compilation

gcc signal7.c -o signal7

### Execution

./signal7

---

## Conclusion

The practical demonstrates different Linux signals and their effects on processes.

The programs show signal handling, default signal behavior, parent-child process signaling, pipe-related signals, process termination, and invalid memory access.
