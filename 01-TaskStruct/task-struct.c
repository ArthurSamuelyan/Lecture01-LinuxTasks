/* Test program to show task struct */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TASK_STRUCT_CDEV "/dev/task-struct-cdev"


void talk_to_kernel()
{
  int cdev_fd = open(TASK_STRUCT_CDEV, O_RDWR, 0);
  if (cdev_fd == -1) {
    printf("Error: open(%s) failed: %d %s\n", 
        TASK_STRUCT_CDEV,
        errno, strerror(errno));
    return;
  }
  read(cdev_fd, NULL, 0);
  printf("Checkout kernel output\n");
  close(cdev_fd);
}

void wait_user_input()
{
  char a;
  scanf("%c", &a);
  printf("\n");
}

void* child_thread_func(void* args)
{
  pid_t parent_pid = getppid();
  printf("Child thread: BORN PID=%d ParentPID=%d\n", getpid(), parent_pid);

  talk_to_kernel();
}

int main(int argc, char** argv)
{
  pthread_t child;

  printf("Parent process: PID=%d ParentPID=%d\n", getpid(), getppid());

  talk_to_kernel();
  wait_user_input();

  if (pthread_create(&child, NULL, child_thread_func, NULL) == -1)
  {
    printf("Error: pthread_create() failed: %d %s\n", errno, strerror(errno));
    exit(1);
  }
  if (pthread_join(child, NULL) != 0)
  {
    printf("Error: pthread_join() failed: %d %s\n", errno, strerror(errno));
    exit(1);
  }
  
  wait_user_input();

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
        talk_to_kernel();
        exit(0);
      }
      break;

    default:
      {
        /* This process is parent */
        wait(NULL);
        wait_user_input();
      }
      break;
  }

  return 0;
}
