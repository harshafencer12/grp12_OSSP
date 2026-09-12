#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n[HANDLER] SIGINT received - Ctrl+C pressed.\n");
    }
    else if (sig == SIGTERM)
    {
        printf("\n[HANDLER] SIGTERM received - terminating program.\n");
        running = 0;
    }
    else if (sig == SIGUSR1)
    {
        printf("\n[HANDLER] SIGUSR1 received - custom user signal.\n");
    }
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    printf("========================================\n");
    printf("      POSIX SIGNAL HANDLING PROGRAM\n");
    printf("========================================\n");

    printf("Process PID: %d\n", getpid());
    printf("Press Ctrl+C to send SIGINT.\n");
    printf("Use another terminal for SIGUSR1:\n");
    printf("kill -USR1 %d\n", getpid());
    printf("Use another terminal for SIGTERM:\n");
    printf("kill -TERM %d\n\n", getpid());

    while (running)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    printf("Program terminated safely.\n");

    return 0;
}
