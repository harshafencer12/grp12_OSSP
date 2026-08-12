#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

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
        printf("PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent waiting for child...\n");

        waitpid(pid, &status, 0);

        printf("Child has been successfully reaped.\n");
        printf("Parent exiting...\n");
    }

    return 0;
}
