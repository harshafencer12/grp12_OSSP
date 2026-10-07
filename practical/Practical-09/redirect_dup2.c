#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
    int fd = open("redirect_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); close(fd); return 1; }

    if (pid == 0) {
        if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2 stdout"); _exit(1); }
        if (dup2(fd, STDERR_FILENO) < 0) { perror("dup2 stderr"); _exit(1); }
        close(fd);
        printf("This stdout line was redirected using dup2().\n");
        fprintf(stderr, "This stderr line was also redirected.\n");
        fflush(stdout);
        _exit(0);
    }

    close(fd);
    waitpid(pid, NULL, 0);

    printf("Parent: child completed. Check redirect_output.txt\n");
    return 0;
}
