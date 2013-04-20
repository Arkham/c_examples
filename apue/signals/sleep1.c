#include "../apue.h"

static void
sig_alrm(int signo)
{
  /* void */
}

unsigned int
sleep1(unsigned int nseconds)
{
  if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    return(nseconds);
  alarm(nseconds);
  pause();
  return(alarm(0));
}

/*
unsigned int
sleep1(unsigned int nsecs)
{
  Sigfunc *prev_handler;
  unsigned int prev_secs;

  if ( (prev_handler = signal(SIGALRM, sig_alrm)) == SIG_ERR)
    return(nsecs);

  prev_secs = alarm(nsecs);
  pause();

  signal(SIGALRM, prev_handler);
  return(alarm(prev_secs));
}
*/

int
main(void)
{
  printf("Start\n");
  sleep1(5);
  printf("Stop\n");
  exit(0);
}
