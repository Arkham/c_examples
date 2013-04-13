#include "../apue.h"
#include <fcntl.h>

int
main(void) {

  if (open("tempfile", O_RDWR) < 0)
    err_sys("open failed");

  if (unlink("tempfile") < 0)
    err_sys("unlink failed");

  printf("file unlinked\n");
  sleep(15);
  printf("done\n");

  exit(0);
}
