#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // Create child process
    pid = fork();

    if (pid < 0) {
        // Error occurred
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // Child process block
        printf("Child Process:\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Message: Hello from the child process!\n");
    } 
    else {
        // Parent process block
        // Wait for child to complete
        wait(NULL);

        printf("Parent Process:\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Message: Child has finished. Hello from the parent process!\n");
    }

    return 0;
}

