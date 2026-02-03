#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Shared data structure
typedef struct {
    int *fib;   // array to hold Fibonacci numbers
    int n;      // number of terms
} FibData;

// Thread function to generate Fibonacci series
void* generateFibonacci(void *arg) {
    FibData *data = (FibData*) arg;

    if (data->n > 0) data->fib[0] = 0;
    if (data->n > 1) data->fib[1] = 1;

    for (int i = 2; i < data->n; i++) {
        data->fib[i] = data->fib[i-1] + data->fib[i-2];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_terms>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Number of terms must be positive.\n");
        return 1;
    }

    // Allocate memory for Fibonacci sequence
    int *fib = malloc(n * sizeof(int));
    if (fib == NULL) {
        perror("malloc failed");
        return 1;
    }

    FibData data = {fib, n};
    pthread_t tid;

    // Create child thread
    pthread_create(&tid, NULL, generateFibonacci, (void*)&data);

    // Wait for child thread to finish
    pthread_join(tid, NULL);

    // Parent prints the sequence
    printf("Fibonacci series (%d terms):\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", fib[i]);
    }
    printf("\n");

    free(fib);
    return 0;
}

