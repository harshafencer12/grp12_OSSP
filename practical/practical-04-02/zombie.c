#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process:\n");
        printf("PID  = %d\n", getpid());
        printf("PPID = %d\n", getppid());

        printf("Child exiting...\n");
        exit(0);
    }
    else
    {
        printf("Parent process:\n");
        printf("PID  = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent sleeping for 120 seconds...\n");
        printf("During this time, the child becomes a zombie.\n");

        sleep(120);

        printf("Parent exiting...\n");
    }

    return 0;
}
