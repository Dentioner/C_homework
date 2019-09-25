#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  
   int count = 1;
   int child;

   child = fork();
 
   if(child < 0) {
      perror("fork error");
   }
   else if (child == 0) {
      printf("child process, count is %d(%p), pid is %d\n", ++count, &count, getpid());
      printf("father process, count is %d(%p), pid is %d\n", count, &count, getppid());
   }
   else {
      sleep(1);
      printf("father process, count is %d(%p), pid is %d\n", count, &count, getpid());
      printf("father process, count is %d(%p), pid is %d\n", count, &count, getppid());
   }

   printf("process is over\n");
}

			
