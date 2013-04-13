#include "../apue.h"

#define BUFSIZE 4096

int
main(void)
{
  char line[BUFSIZE];

  while ( fgets(line, BUFSIZE, stdin) != NULL)
    if (fputs(line, stdout) == EOF)
      err_sys("output error");

  if (ferror(stdin))
    err_sys("input error");

  exit(0);
}
