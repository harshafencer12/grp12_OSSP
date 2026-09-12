#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handle_sigint(int sig)
{
    printf("\n[HANDLER] SIGINT received!\n");
}

void handle_sigchld(int sig)
{
    printf("[HANDLER] SIGCHLD received: Child state changed.\n");
}

void handle_sigusr1(int sig)
{
    printf("[HANDLER] Child received SIGUSR1.\n");
}

void demonstrate_sigint()
{
    printf("\n========== SIGINT ==========\n");

    signal(SIGINT, handle_sigint);

    printf("SIGINT handler installed.\n");
    printf("Press Ctrl+C to test SIGINT.\n");
    printf("Waiting for 3 seconds...\n");

    sleep(3);

    printf("SIGINT demonstration completed.\n");
}

void demonstrate_sigchld()
{
    printf("\n========== SIGCHLD ==========\n");

    signal(SIGCHLD, handle_sigchld);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("Child process running...\n");
        sleep(2);
        printf("Child process terminating...\n");
        exit(0);
    }
    else
    {
        printf("Parent created child with PID: %d\n", pid);

        waitpid(pid, NULL, 0);

        printf("Parent finished waiting for child.\n");
    }
}

void demonstrate_sigpipe()
{
    printf("\n========== SIGPIPE ==========\n");

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe failed");
        return;
    }

    close(fd[0]);

    printf("Read end of pipe closed.\n");
    printf("Child will attempt to write to the pipe.\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        signal(SIGPIPE, SIG_IGN);

        ssize_t result = write(fd[1], "Hello", 5);

        if (result == -1)
        {
            perror("write");
            printf("SIGPIPE condition detected.\n");
        }

        close(fd[1]);
        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
        close(fd[1]);

        printf("SIGPIPE demonstration completed.\n");
    }
}

void demonstrate_kill()
{
    printf("\n========== kill() + SIGUSR1 ==========\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        signal(SIGUSR1, handle_sigusr1);

        printf("Child waiting for SIGUSR1...\n");

        sleep(5);

        printf("Child exiting.\n");
        exit(0);
    }
    else
    {
        sleep(1);

        printf("Parent sending SIGUSR1 to child...\n");

        kill(pid, SIGUSR1);

        sleep(1);

        printf("Parent sending SIGTERM to child...\n");

        kill(pid, SIGTERM);

        waitpid(pid, NULL, 0);

        printf("Child terminated.\n");
    }
}

void demonstrate_sigabrt()
{
    printf("\n========== SIGABRT ==========\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        printf("Child process started.\n");
        printf("Calling abort()...\n");

        abort();

        printf("This line will not execute.\n");
    }
    else
    {
        waitpid(pid, NULL, 0);

        printf("Parent detected SIGABRT termination.\n");
    }
}

void demonstrate_sigsegv()
{
    printf("\n========== SIGSEGV ==========\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        int *p = NULL;

        printf("Child process started.\n");
        printf("Attempting invalid memory access...\n");

        *p = 10;

        printf("This line will not execute.\n");
    }
    else
    {
        waitpid(pid, NULL, 0);

        printf("Parent detected SIGSEGV termination.\n");
    }
}

int main()
{
    printf("========================================\n");
    printf("       LINUX SIGNALS DEMONSTRATION      \n");
    printf("========================================\n");

    demonstrate_sigint();

    demonstrate_sigchld();

    demonstrate_sigpipe();

    demonstrate_kill();

    demonstrate_sigabrt();

    demonstrate_sigsegv();

    printf("\n========================================\n");
    printf("       ALL SIGNALS DEMONSTRATED         \n");
    printf("========================================\n");

    return 0;
}
