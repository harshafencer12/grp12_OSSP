#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define REQUEST_FIFO "/tmp/ossp_request_fifo"
#define BUFFER_SIZE 256

typedef struct
{
    pid_t client_pid;
    char message[BUFFER_SIZE];
} Request;

int main()
{
    int fd;
    Request request;
    char response_fifo[100];
    char response[BUFFER_SIZE];

    printf("=====================================\n");
    printf("       FIFO SERVER STARTED\n");
    printf("=====================================\n");

    mkfifo(REQUEST_FIFO, 0666);

    printf("Waiting for client messages...\n");
    printf("Press Ctrl+C to stop the server.\n\n");

    fd = open(REQUEST_FIFO, O_RDWR);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    while (1)
    {
        ssize_t bytes = read(fd, &request, sizeof(Request));

        if (bytes > 0)
        {
            printf("Received from Client PID %d: %s\n",
                   request.client_pid, request.message);

            snprintf(response, BUFFER_SIZE,
         "Server processed your message: %.220s",
         request.message);

            snprintf(response_fifo, sizeof(response_fifo),
                     "/tmp/ossp_response_fifo_%d",
                     request.client_pid);

            int response_fd = open(response_fifo, O_WRONLY);

            if (response_fd != -1)
            {
                write(response_fd, response, strlen(response) + 1);
                close(response_fd);
            }
            else
            {
                perror("Could not open client response FIFO");
            }
        }
    }

    close(fd);
    unlink(REQUEST_FIFO);

    return 0;
}
