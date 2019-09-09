/* Test program to perform zombie task */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv)
{
  pid_t child_pid = fork();

  switch (child_pid) {
    case -1:
      {
        printf("Error: fork() failed: %d %s\n", errno, strerror(errno));
        exit(1);
      }
      break;

    case 0:
      {
        /* This process is child */
        printf("Child process: PID=%d ParentPID=%d\n", getpid(), getppid());
        exit(0);
      }
      break;

    default:
      {
        /* This process is parent */
        printf("Parent process: PID=%d\n", getpid());

        /* Normally here shall be
         * wait(),
         * but this test shall perform zombie task. 
         *
         * Hence parent shall not exit 
         * (in order to not allow reparenting) */
        while(1)
        {}
      }
      break;
  }
  return 0;
}
