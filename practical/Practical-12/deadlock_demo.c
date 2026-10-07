#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;

static void *thread1(void *arg) {
    (void)arg;
    pthread_mutex_lock(&A);
    printf("T1 locked A\n");
    sleep(1);
    printf("T1 waiting for B...\n");
    pthread_mutex_lock(&B);
    printf("T1 locked B\n");
    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);
    return NULL;
}

static void *thread2(void *arg) {
    (void)arg;
    pthread_mutex_lock(&B);
    printf("T2 locked B\n");
    sleep(1);
    printf("T2 waiting for A...\n");
    pthread_mutex_lock(&A);
    printf("T2 locked A\n");
    pthread_mutex_unlock(&A);
    pthread_mutex_unlock(&B);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    puts("Intentional deadlock demo. It will not terminate normally.");
    puts("Run with: timeout 5s ./deadlock_demo");
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
