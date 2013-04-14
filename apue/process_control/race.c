#include "../apue.h"

static void characteratatime(char *);

int
main(void)
{
  pid_t pid;

  if ( (pid = fork()) < 0 ) {
    err_sys("fork error");
  } else if (pid == 0) {
    sleep(1);
    /* WAIT_PARENT() */
    characteratatime("output from child\n");
  } else {
    sleep(1);
    characteratatime("output from parent\n");
    /* TELL_CHILD(pid) */
  }

  exit(0);
}

static void
characteratatime(char *str)
{
  char *ptr = str;
  int c;

  setbuf(stdout, NULL); /* set stdout to unbuffered */

  while (( c = *ptr++ ) != 0) {
    putc(c, stdout);
  }
}
