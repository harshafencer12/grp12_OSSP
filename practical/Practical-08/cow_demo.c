#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define N (1024 * 1024)

static void print_rss(const char *label) {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            printf("%s: %s", label, line);
            break;
        }
    }
    fclose(f);
}

int main(void) {
    char *data = malloc(N);
    if (!data) { perror("malloc"); return 1; }
    memset(data, 'A', N);

    printf("Parent PID: %d\n", getpid());
    print_rss("Parent before fork");

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); free(data); return 1; }

    if (pid == 0) {
        printf("Child PID: %d\n", getpid());
        print_rss("Child immediately after fork");
        printf("Child modifies one page...\n");
        data[0] = 'C';
        print_rss("Child after modification");
        printf("Child data[0] = %c\n", data[0]);
        _exit(0);
    }

    waitpid(pid, NULL, 0);
    printf("Parent data[0] = %c\n", data[0]);
    print_rss("Parent after child exits");
    free(data);
    puts("COW idea: after fork(), pages are initially shared; a write causes a private copy of the modified page.");
    return 0;
}
