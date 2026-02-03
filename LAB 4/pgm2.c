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
        printf("Child Process (before exec):\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Message: Now loading the previous program using exec...\n");

        // Replace child process image with the previous program
        // Assume the previous program was compiled as "childprog"
        execl("./childprog", "childprog", (char *)NULL);

        // If execl fails
        perror("execl failed");
        exit(1);
    } 
    else {
        // Parent process block
        // Wait for child to complete
        wait(NULL);

        printf("Parent Process:\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Message: Child has finished executing the binary.\n");
    }

    return 0;
}

