#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // Child process
        printf("Child process:\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Exiting now to become zombie...\n");
        exit(0);  // Child exits immediately
    } 
    else {
        // Parent process
        printf("Parent process:\n");
        printf("  PID: %d\n", getpid());
        printf("  PPID: %d\n", getppid());
        printf("  Sleeping... not calling wait()\n");

        // Sleep long enough to observe zombie state
        sleep(30);

        printf("Parent terminating... child will be adopted by init.\n");
    }

    return 0;
}

