#include "../apue.h"

int
main(void)
{
  printf("Hello world, this is process %d\n", getpid());
  exit(0);
}
