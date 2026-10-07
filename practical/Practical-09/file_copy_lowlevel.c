#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 65536

static int copy_file(const char *src, const char *dst) {
    int in = open(src, O_RDONLY);
    if (in < 0) { perror("open source"); return -1; }

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { perror("open destination"); close(in); return -1; }

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        ssize_t sent = 0;
        while (sent < n) {
            ssize_t w = write(out, buf + sent, (size_t)(n - sent));
            if (w < 0) {
                if (errno == EINTR) continue;
                perror("write");
                close(in); close(out);
                return -1;
            }
            sent += w;
        }
    }
    if (n < 0) perror("read");

    if (close(in) < 0) perror("close input");
    if (close(out) < 0) perror("close output");
    return n < 0 ? -1 : 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    return copy_file(argv[1], argv[2]) == 0 ? 0 : 1;
}
