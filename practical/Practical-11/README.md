# Practical 11

## Race-condition version
```bash
gcc -Wall -Wextra -pthread race_counter.c -o race_counter
./race_counter
```
The unsynchronized shared increment has a data race. The actual value may be lower than the expected value.

## Mutex version
```bash
gcc -Wall -Wextra -pthread mutex_counter.c -o mutex_counter
./mutex_counter
```
The mutex serializes the critical section, so the final count should equal the expected value.

Run several times to demonstrate the difference.
