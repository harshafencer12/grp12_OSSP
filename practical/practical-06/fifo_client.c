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
    int request_fd;
    int response_fd;
    Request request;
    char response_fifo[100];
    char response[BUFFER_SIZE];

    request.client_pid = getpid();

    snprintf(response_fifo, sizeof(response_fifo),
             "/tmp/ossp_response_fifo_%d",
             request.client_pid);

    mkfifo(response_fifo, 0666);

    printf("=====================================\n");
    printf("           FIFO CLIENT\n");
    printf("=====================================\n");

    printf("Enter message: ");
    fflush(stdout);

    fgets(request.message, BUFFER_SIZE, stdin);

    request.message[strcspn(request.message, "\n")] = '\0';

    request_fd = open(REQUEST_FIFO, O_WRONLY);

    if (request_fd == -1)
    {
        perror("Could not connect to server");
        unlink(response_fifo);
        return 1;
    }

    write(request_fd, &request, sizeof(Request));

    close(request_fd);

    printf("Message sent to server.\n");
    printf("Waiting for response...\n");

    response_fd = open(response_fifo, O_RDONLY);

    if (response_fd == -1)
    {
        perror("Could not open response FIFO");
        unlink(response_fifo);
        return 1;
    }

    read(response_fd, response, BUFFER_SIZE);

    printf("Server Response: %s\n", response);

    close(response_fd);
    unlink(response_fifo);

    return 0;
}
