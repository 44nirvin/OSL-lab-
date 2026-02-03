#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// Structure to pass arguments to threads
typedef struct {
    int *arr;
    int n;
    char algo_name[20];
} SortArgs;

// Utility function to print array
void printArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Bubble Sort
void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Insertion Sort
void insertionSort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// Quick Sort
int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;
    return i+1;
}

void quickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

// Thread function
void* sortThread(void *args) {
    SortArgs *sargs = (SortArgs*) args;

    // Copy array to avoid modifying original
    int *arr_copy = malloc(sargs->n * sizeof(int));
    memcpy(arr_copy, sargs->arr, sargs->n * sizeof(int));

    clock_t start = clock();

    if (strcmp(sargs->algo_name, "Bubble Sort") == 0) {
        bubbleSort(arr_copy, sargs->n);
    } else if (strcmp(sargs->algo_name, "Insertion Sort") == 0) {
        insertionSort(arr_copy, sargs->n);
    } else if (strcmp(sargs->algo_name, "Quick Sort") == 0) {
        quickSort(arr_copy, 0, sargs->n - 1);
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n%s Result:\n", sargs->algo_name);
    printArray(arr_copy, sargs->n);
    printf("%s Time Taken: %f seconds\n", sargs->algo_name, time_taken);

    free(arr_copy);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <number_of_elements> <elements...>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (argc != n + 2) {
        printf("Error: You must provide %d elements.\n", n);
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i+2]);
    }

    pthread_t threads[3];
    SortArgs args[3] = {
        {arr, n, "Bubble Sort"},
        {arr, n, "Insertion Sort"},
        {arr, n, "Quick Sort"}
    };

    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, sortThread, (void*)&args[i]);
    }

    // Wait for all threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Final sorted array using Quick Sort (as reference)
    quickSort(arr, 0, n-1);
    printf("\nFinal Sorted Array (Quick Sort reference):\n");
    printArray(arr, n);

    free(arr);
    return 0;
}

