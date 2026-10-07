#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;

/* Resource ordering: every thread locks A before B. */
static void *worker(void *arg) {
    long id = (long)arg;

    pthread_mutex_lock(&A);
    printf("T%ld locked A\n", id);
    usleep(100000);

    pthread_mutex_lock(&B);
    printf("T%ld locked B\n", id);
    printf("T%ld using both resources\n", id);

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, worker, (void *)1);
    pthread_create(&t2, NULL, worker, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    puts("Completed without deadlock because resource ordering is consistent.");
    pthread_mutex_destroy(&A);
    pthread_mutex_destroy(&B);
    return 0;
}
