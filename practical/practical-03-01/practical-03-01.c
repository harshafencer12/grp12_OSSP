#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    printf("Before fork()\n");
    printf("Current Process ID (PID): %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        printf("Fork Failed!\n");
        return 1;
    }
    else if (pid == 0)
    {
        // Child Process
        printf("\n----- Child Process -----\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());

        printf("Child is sleeping for 5 seconds...\n");
        sleep(5);

        printf("Child Process Finished.\n");
    }
    else
    {
        // Parent Process
        printf("\n----- Parent Process -----\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n", pid);

        printf("Parent is sleeping for 20 seconds...\n");
        sleep(20);
        printf("Parent Process Finished.\n");
    }

    return 0;
}
