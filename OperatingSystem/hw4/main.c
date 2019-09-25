#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int  main()
{
     pid_t  pid;
     int  i;
	printf("the initial pid = %d, ppid = %d\n", getpid(), getppid());
     
     for  (i=0;  i<3;  i++)
      {
           /* fork  another  process  */
           pid = fork();
           if  (pid < 0) { /*error  occurred  */
                fprintf(stderr, "Fork Failed");
                exit(-1);
           }
           else if (pid == 0) { /* child process */
                fprintf(stdout,  "i=%d,  pid=%d,  parent  pid=%d\n",i,      
                getpid() ,getppid());
           }
	   else
	   {
		   printf("this is parent, i = %d, pid = %d\n", i, getpid());
	   }
	
      }

     
	int ppiidd =  wait(NULL);
	printf("wait = %d\n", ppiidd);
      exit(0);
} 
