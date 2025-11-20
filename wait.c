#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
        pid_t pid = 0;
        int status;

        pid = fork();
        printf("PID: %d\n", pid);


        if (pid == 0) // in child
        {
                sleep(3);
        }
        if (pid > 0) { // parent
                pid = wait(&status);
                printf("Parent sees that process %d finished with status %d!\n", pid, status);
        }

        return 0;
