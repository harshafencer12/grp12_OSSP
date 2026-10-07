# Practical 8

## Part A — malloc/calloc/realloc/free
```bash
gcc -Wall -Wextra -g memory_alloc.c -o memory_alloc
./memory_alloc
valgrind --leak-check=full ./memory_alloc
```
Expected Valgrind result: all allocated blocks should be freed.

## Part B — Copy-on-Write
```bash
gcc -Wall -Wextra -g cow_demo.c -o cow_demo
./cow_demo
```
`fork()` creates a child with logically copied address space. Linux can initially share physical pages and mark them copy-on-write. When the child writes to a shared page, a private copy is created.
