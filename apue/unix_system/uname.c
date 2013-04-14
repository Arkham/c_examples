#include "../apue.h"
#include "sys/utsname.h"

int
main(void)
{
  struct utsname *ptr;

  if (uname(ptr) < 0)
    err_sys("uname failed");

  printf("%s %s ", ptr->sysname, ptr->nodename);
  printf("%s %s ", ptr->release, ptr->version);
  printf("%s\n", ptr->machine);

  exit(0);
}
