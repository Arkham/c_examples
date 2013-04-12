#include "../apue.h"
#include "fnctl.h"

char buf1[] = "abcdef";
char buf2[] = "ABCDEF";

int
main(void)
{

  int fd;

  if ( (fd = creat("file.hole", FILE_MODE)) < 0)
    err_sys("creat error");

  if (write(fd, buf1, 6) != 6)
    err_sys("buf1 write error");

  if (lseek(fd, 14444, SEEK_SET) == -1)
    err_sys("cannot lseek");

  if (write(fd, buf2, 6) != 6)
    err_sys("buf2 write error");

  exit(0);
}
