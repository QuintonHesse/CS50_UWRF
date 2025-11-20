#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
   pid_t pid = 0;
   int status;

   pid = fork();

   if (pid == 0) // In child
   {
      char* argv[3] = {"echo", "echo executed by child!!", NULL};
      execvp("echo", argv);
   }
   if (pid > 0)
   {
      pid = wait(&status);
      printf("Parent sees that process %d finished with status %d!\n", pid, status);
   }
   if (pid < 0)
   {
      fprintf(stderr, "Fork failed!\n");
   }

   return 0;
}
