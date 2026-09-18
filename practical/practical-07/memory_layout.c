#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Global variable */
int global_variable = 100;

/* Static global variable */
static int static_global_variable = 200;

/* Function - code segment */
void sample_function()
{
    printf("Inside sample_function()\n");
}

int main()
{
    /* Local variable - stack */
    int stack_variable = 300;

    /* Static local variable - data segment */
    static int static_local_variable = 400;

    /* Heap variable */
    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc");
        return 1;
    }

    *heap_variable = 500;

    printf("============================================\n");
    printf("       LINUX PROCESS MEMORY LAYOUT\n");
    printf("============================================\n");

    printf("Process PID              : %d\n", getpid());

    printf("\nMemory Addresses:\n");
    printf("--------------------------------------------\n");

    printf("Code (function)          : %p\n",
           (void *)sample_function);

    printf("Global variable          : %p\n",
           (void *)&global_variable);

    printf("Static global variable   : %p\n",
           (void *)&static_global_variable);

    printf("Static local variable    : %p\n",
           (void *)&static_local_variable);

    printf("Heap variable            : %p\n",
           (void *)heap_variable);

    printf("Stack variable           : %p\n",
           (void *)&stack_variable);

    printf("--------------------------------------------\n");

    printf("\nMemory Segment Analysis:\n");
    printf("Code   -> Contains program instructions.\n");
    printf("Global -> Stores global variables.\n");
    printf("Static -> Stores static variables.\n");
    printf("Heap   -> Dynamically allocated memory.\n");
    printf("Stack  -> Stores local variables and function data.\n");

    printf("\nHeap value  : %d\n", *heap_variable);
    printf("Stack value : %d\n", stack_variable);

    printf("\nProcess is running...\n");
    printf("PID: %d\n", getpid());
    printf("Open another terminal and run:\n");
    printf("cat /proc/%d/maps\n", getpid());
    printf("pmap %d\n", getpid());

    printf("\nPress Enter to terminate...\n");
    getchar();

    free(heap_variable);

    printf("Heap memory released successfully.\n");

    return 0;
}
