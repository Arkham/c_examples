#include "../apue.h"

static void
my_alrm(int signo)
{
  /* void */
}

unsigned int
my_sleep(unsigned int nsecs)
{
  struct sigaction newact, oldact;
  sigset_t newmask, oldmask, suspmask;
  unsigned int unslept;

  /* set up new handler, save old information */
  newact.sa_handler = my_alrm;
  sigemptyset(&newact.sa_mask);
  newact.sa_flags = 0;
  sigaction(SIGALRM, &newact, &oldact);

  /* block SIGALRM and save current signal mask */
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGALRM);
  sigprocmask(SIG_BLOCK, &newmask, &oldmask);

  alarm(nsecs);

  suspmask = oldmask;
  sigdelset(&suspmask, SIGALRM); /* make sure SIGALRM isn't blocked */
  sigsuspend(&suspmask);         /* sleep waiting for any signal */

  unslept = alarm(0);            /* stop alarm and read seconds left */
  sigaction(SIGALRM, &oldact, NULL);

  sigprocmask(SIG_SETMASK, &oldmask, NULL);
  return(unslept);
}

int
main(void)
{
  printf("Sleep 2 seconds\n");
  my_sleep(2);

  printf("Sleep 1 second\n");
  my_sleep(1);

  printf("Sleep 1 second\n");
  my_sleep(1);

  printf("Sleep 1 second\n");
  my_sleep(1);

  exit(0);
}
