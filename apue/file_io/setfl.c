#include "../apue.h"
#include "fcntl.h"

void
set_fl(int fd, int flags)
{
  int val;

  if ( (val = fcntl(fd, F_GETFL, 0)) < 0 )
    err_sys("fcntl F_GETFL error");

   val |= flags;

  if (fcntl(fd, F_SETFL, val) < 0)
    err_sys("fcntl F_SETFL error");
}

#define BUFSIZE 4096

int
main(void)
{
  int n;
  char buf[BUFSIZE];

  set_fl(STDOUT_FILENO, O_SYNC);

  if ( (n = read(STDIN_FILENO, buf, BUFSIZE)) < 0 )
    err_sys("read error");

  if (write(STDERR_FILENO, buf, n) != n)
    err_sys("write error");

  exit(0);
}
