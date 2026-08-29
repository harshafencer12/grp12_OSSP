# Skill-03 Task-02: Dynamic Memory Management

## Objective

To dynamically allocate buffers, resize arrays, prevent buffer overflow, manage linked lists, release memory correctly, and verify memory usage using Valgrind.

## Features

- Dynamic memory allocation using `malloc()`
- Dynamic array resizing using `realloc()`
- Bounds checking before inserting data
- Linked-list creation
- Dynamic node allocation
- Proper memory deallocation using `free()`
- Valgrind memory verification

## Dynamic Array

The program initially allocates memory for two integers.

```c
int *array = malloc(size * sizeof(int));
