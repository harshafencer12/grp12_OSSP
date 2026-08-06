#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid1, pid2;

    pid1 = fork();

    if (pid1 == 0)
    {
        printf("Child 1 Started\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());

        sleep(3);

        printf("Child 1 Finished\n");
        exit(1);
    }

    pid2 = fork();

    if (pid2 == 0)
    {
        printf("\nChild 2 Started\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());

        sleep(5);

        printf("Child 2 Finished\n");
        exit(2);
    }

    int status;

    printf("\nParent Waiting using wait()...\n");

    wait(&status);

    printf("One child has terminated.\n");

    printf("\nParent Waiting for Child 2 using waitpid()...\n");

    waitpid(pid2, &status, 0);

    printf("Child 2 has terminated.\n");

    printf("\nParent Process Finished.\n");

    return 0;
}
