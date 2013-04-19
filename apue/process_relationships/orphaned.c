#include "../apue.h"
#include <errno.h>

static void
sig_hup(int signo)
{
  printf("SIGHUP received, pid = %d\n", getpid());
}

static void
pr_ids(char *name)
{
  printf("%s: pid = %d, ppid = %d, pgrp = %d, tpgrp = %d\n",
      name, getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_FILENO));
  fflush(stdout);
}

int
main(void)
{
  char c;
  pid_t pid;

  pr_ids("parent");

  if ( (pid = fork()) < 0 ) {
    err_sys("fork error");
  } else if (pid == 0) {
    pr_ids("child");

    signal(SIGHUP, sig_hup); /* signal handler */
    kill(getpid(), SIGTSTP); /* suspends itself */

    pr_ids("child");         /* this is printed when we are resumed */
    if (read(STDIN_FILENO, &c, 1) != 1)
      printf("read error from controlling tty, errno = %d\n", errno);
  } else {
    /* parent */
    sleep(5);
  }

  exit(0);
}
