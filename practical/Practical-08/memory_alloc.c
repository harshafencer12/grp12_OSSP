#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("malloc/calloc/realloc/free demonstration\n");

    int *a = malloc(5 * sizeof(*a));
    if (!a) { perror("malloc"); return 1; }
    for (int i = 0; i < 5; i++) a[i] = (i + 1) * 10;
    printf("malloc: ");
    for (int i = 0; i < 5; i++) printf("%d ", a[i]);
    puts("");

    int *b = calloc(5, sizeof(*b));
    if (!b) { perror("calloc"); free(a); return 1; }
    printf("calloc initially: ");
    for (int i = 0; i < 5; i++) printf("%d ", b[i]);
    puts("");

    int *tmp = realloc(a, 10 * sizeof(*a));
    if (!tmp) { perror("realloc"); free(a); free(b); return 1; }
    a = tmp;
    for (int i = 5; i < 10; i++) a[i] = (i + 1) * 10;
    printf("realloc: ");
    for (int i = 0; i < 10; i++) printf("%d ", a[i]);
    puts("");

    free(a);
    free(b);
    puts("free: all dynamically allocated blocks released.");
    return 0;
}
