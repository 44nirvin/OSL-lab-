#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t limit;          // Controls producer lead
pthread_mutex_t lock; // Only for clean printing

int produced = 0;
int consumed = 0;

void *producer(void *arg) {
    while (1) {
        sem_wait(&limit);   // Check if producer can run ahead

        pthread_mutex_lock(&lock);

        produced++;
        printf("Produced item %d\n", produced);

        pthread_mutex_unlock(&lock);

        sleep(1);  // Simulate work
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        consumed++;
        printf("Consumed item %d\n", consumed);

        pthread_mutex_unlock(&lock);

        sem_post(&limit);   // Allow producer to produce more

        sleep(2);  // Simulate slower consumption
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    sem_init(&limit, 0, 10);     // Producer can be 10 ahead max
    pthread_mutex_init(&lock, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&limit);
    pthread_mutex_destroy(&lock);

    return 0;
}
