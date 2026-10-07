#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ITEMS 100000
#define BUFFER_SIZE 16

static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static sem_t empty_slots, full_slots;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full_slots);
    }
    return NULL;
}

static void *consumer(void *arg) {
    (void)arg;
    long sum = 0;
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&full_slots);
        pthread_mutex_lock(&mutex);
        int value = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);
        sum += value;
    }
    printf("Consumer received %d items, checksum=%ld\n", ITEMS, sum);
    return NULL;
}

int main(void) {
    pthread_t p, c;

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mutex);
    return 0;
}
