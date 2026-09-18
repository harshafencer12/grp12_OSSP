#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char path[64];
    char line[512];

    printf("============================================\n");
    printf("       /proc/PID/maps MEMORY ANALYSIS\n");
    printf("============================================\n");

    printf("Process PID: %d\n", getpid());

    snprintf(path, sizeof(path),
             "/proc/%d/maps", getpid());

    FILE *fp = fopen(path, "r");

    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    printf("\nVirtual Memory Mappings:\n");
    printf("--------------------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    printf("--------------------------------------------\n");

    fclose(fp);

    printf("\nAnalysis:\n");
    printf("[heap]  -> Dynamic memory allocation area.\n");
    printf("[stack] -> Process stack area.\n");
    printf("r-xp    -> Read and execute permissions.\n");
    printf("rw-p    -> Read and write permissions.\n");
    printf("Shared libraries are mapped into the process address space.\n");

    return 0;
}
