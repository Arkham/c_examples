#include "../apue.h"

#define BUFSIZE 4096

int
main(void)
{
  char path[BUFSIZE];
  if (chdir("/tmp") < 0)
    err_sys("chdir failed");

  printf("chdir to /tmp successful!\n");

  if (getcwd(path, BUFSIZE) == NULL)
    err_sys("getcwd failed");

  printf("current working path is: %s\n", path);

  exit(0);
}
