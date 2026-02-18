#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;   // Protects readcount
sem_t wrt;     // Writer lock (database access)

int readcount = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(&mutex);

        readcount++;
        if (readcount == 1)
            sem_wait(&wrt);   // First reader blocks writers

        sem_post(&mutex);

        // Critical Section (Reading) 
        printf("Reader %d is reading\n", id);
        sleep(1);

        sem_wait(&mutex);

        readcount--;
        if (readcount == 0)
            sem_post(&wrt);   // Last reader releases writer lock

        sem_post(&mutex);

        sleep(1);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(&wrt);

        //  Critical Section (Writing) 
        printf("Writer %d is writing\n", id);
        sleep(2);

        sem_post(&wrt);

        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t r[3], w[2];
    int rid[3] = {1, 2, 3};
    int wid[2] = {1, 2};

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    for (int i = 0; i < 3; i++)
        pthread_create(&r[i], NULL, reader, &rid[i]);

    for (int i = 0; i < 2; i++)
        pthread_create(&w[i], NULL, writer, &wid[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    return 0;
}
