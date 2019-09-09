/* Test program to perform reparenting */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv)
{
  pid_t child_pid = fork();
  pid_t parent_pid;

  switch (child_pid)
  {
    case -1:
      {
        printf("Error: fork() failed: %d %s\n", errno, strerror(errno));
        exit(1);
      }
      break;

    case 0:
      {
        /* This process is child */
        parent_pid = getppid();
        printf("Child process: BORN PID=%d ParentPID=%d\n", getpid(), parent_pid);
        
        while (1)
        {
          if (parent_pid != getppid())
          {
            parent_pid = getppid();
            printf("Child process: PARENT_CHANGED PID=%d ParentPID=%d\n", 
                getpid(), parent_pid);
          }
        }
      }
      break;

    default:
      {
        /* This process is parent */
        printf("Parent process: PID=%d\n", getpid());

        while(1)
        {}
      }
      break;
  }
  return 0;
}
