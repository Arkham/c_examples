#include "../apue.h"
#include <signal.h>

#define BUFFSIZE 1024

static void sig_tstp(int signo);

int
main(void)
{
  int n;
  char buf[BUFFSIZE];

  /* catch SIGTSTP only if shell supports job control */
  if (signal(SIGTSTP, SIG_IGN) == SIG_DFL)
    signal(SIGTSTP, sig_tstp);

  while ( (n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0 ) {
    if (write(STDOUT_FILENO, buf, n) != n)
      err_sys("write error");
  }

  if (n < 0)
    err_sys("read error");

  exit(0);
}

static void
sig_tstp(int signo)
{
  sigset_t mask;

  /* here we should handle terminal change, move cursor, reset tty mode */
  printf("see you later...\n");

  /* unblock SIGTSTP, since it's blocked because we are handling it */
  sigemptyset(&mask);
  sigaddset(&mask, SIGTSTP);
  sigprocmask(SIG_UNBLOCK, &mask, NULL);

  /* reset disposition to default and send SIGTSTP to ourselves */
  signal(SIGTSTP, SIG_DFL);
  kill(getpid(), SIGTSTP);

  /* here we have resumed */
  /* re-establish handler */
  signal(SIGTSTP, sig_tstp);

  /* here we should redraw screen, reset tty mode.. */
  printf("..and we're back!\n");
}
