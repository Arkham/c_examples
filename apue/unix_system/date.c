#include "../apue.h"
#include <time.h>

#define BUFSIZE 256

int
main(void)
{
  char result[BUFSIZE];
  time_t now;
  struct tm *ptr;

  now = time(NULL);
  ptr = localtime(&now);

  if (strftime(result, BUFSIZE, "%a %b %d %T %Z %Y", ptr) < 0)
    err_sys("strftime error");

  printf("%s\n", result);

  exit(0);
}
