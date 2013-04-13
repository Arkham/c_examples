#include "../apue.h"

void
check_foo() {
  system("ls -la foo");
}

int
main(void)
{
  struct stat buf;

  if (stat("foo", &buf) < 0)
    err_sys("stat error");

  check_foo();

  if (chmod("foo", (buf.st_mode & ~S_IRGRP) | S_IWOTH) < 0)
    err_sys("chmod error");

  check_foo();

  if (chmod("foo", S_IRUSR | S_IWUSR) < 0)
    err_sys("chmod error");

  check_foo();

  exit(0);
}
