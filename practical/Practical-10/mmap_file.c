#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    const char *name = "mmap_demo.txt";
    const char *initial = "Hello from mmap!\n";
    size_t len = strlen(initial);

    int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    if (ftruncate(fd, (off_t)len) < 0) { perror("ftruncate"); close(fd); return 1; }
    if (write(fd, initial, len) != (ssize_t)len) { perror("write"); close(fd); return 1; }

    char *p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    printf("Before: %.*s", (int)len, p);
    const char replacement[] = "MMAP changed!\n";
    memcpy(p, replacement, len);
    if (msync(p, len, MS_SYNC) < 0) perror("msync");
    printf("After:  %.*s", (int)len, p);

    munmap(p, len);
    close(fd);

    puts("Read the file after unmapping:");
    int r = open(name, O_RDONLY);
    if (r >= 0) {
        char buf[128] = {0};
        ssize_t n = read(r, buf, sizeof(buf)-1);
        if (n > 0) write(STDOUT_FILENO, buf, (size_t)n);
        close(r);
    }
    return 0;
}
