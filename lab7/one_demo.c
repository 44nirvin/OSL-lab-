#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t limit;   
sem_t items;   

pthread_mutex_t lock;

int produced = 0;
int consumed = 0;

void *producer(void *arg) {
    while (1) {
        sem_wait(&limit);    // Enforces (Produced - Consumed) ≤ 10
        pthread_mutex_lock(&lock);
        produced++;
        printf("Produced: %d\n", produced);

        pthread_mutex_unlock(&lock);

        sem_post(&items);    // Add item for consumer
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&items);    // Waits for available item
        pthread_mutex_lock(&lock);
        consumed++;
        printf("Consumed: %d\n", consumed);

        pthread_mutex_unlock(&lock);

        sem_post(&limit);    // Allows producer to move ahead
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    sem_init(&limit, 0, 10);  // Producer max lead = 10
    sem_init(&items, 0, 0);   // nothing produced yet

    pthread_mutex_init(&lock, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
