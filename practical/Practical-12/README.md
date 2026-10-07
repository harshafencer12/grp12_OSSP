# Practical 12

## Producer-consumer with POSIX semaphores + pthreads
```bash
gcc -Wall -Wextra -pthread producer_consumer.c -o producer_consumer
time ./producer_consumer
```

Change `BUFFER_SIZE` to values such as 4, 16, 64, and compare runtime. The producer waits on `empty_slots`; the consumer waits on `full_slots`; the mutex protects `in`, `out`, and the buffer.

## Intentional deadlock
```bash
gcc -Wall -Wextra -pthread deadlock_demo.c -o deadlock_demo
timeout 5s ./deadlock_demo
```
Two threads acquire resources in opposite order:
- T1: A -> B
- T2: B -> A

The four Coffman conditions are:
1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

## Deadlock prevention using resource ordering
```bash
gcc -Wall -Wextra -pthread deadlock_prevention.c -o deadlock_prevention
./deadlock_prevention
```
Both threads acquire A before B. This removes circular wait, so the deadlock cannot form.
