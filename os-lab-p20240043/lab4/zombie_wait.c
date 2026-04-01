#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        printf("Child (PID %d): I'm done. Exiting now.\n", getpid());
        exit(0);
    }

    printf("Parent (PID %d): Child PID is %d. Sleeping 10 seconds before wait().\n", getpid(), pid);
    sleep(10);
    wait(NULL);
    printf("Parent: Called wait(). Zombie should be gone now.\n");
    sleep(10);

    return 0;
}
