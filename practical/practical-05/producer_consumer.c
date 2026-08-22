#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int main()
{
    int pipefd[2];
    pid_t pid;

    char buffer[100];
    int data[] = {10, 20, 30, 40, 50};
    int n = sizeof(data) / sizeof(data[0]);

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid > 0)
    {
        /* Parent - Producer */
        close(pipefd[0]);

        printf("Producer (Parent): Generating data...\n");

        for (int i = 0; i < n; i++)
        {
            int start = clock();

            write(pipefd[1], &data[i], sizeof(data[i]));

            int end = clock();

            printf("Produced: %d | Communication time: %d clock ticks\n",
                   data[i], end - start);
        }

        close(pipefd[1]);

        wait(NULL);

        printf("Producer: Finished.\n");
    }
    else
    {
        /* Child - Consumer */
        close(pipefd[1]);

        int value;

        printf("Consumer (Child): Waiting for data...\n");

        while (read(pipefd[0], &value, sizeof(value)) > 0)
        {
            printf("Consumed: %d\n", value);
        }

        close(pipefd[0]);

        printf("Consumer: Finished.\n");
    }

    return 0;
}
