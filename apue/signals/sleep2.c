#include "../apue.h"
#include <setjmp.h>

static jmp_buf env_alrm;

static void
sig_alrm(int signo)
{
  longjmp(env_alrm, 1);
}

unsigned int
sleep2(unsigned int nsecs)
{
  if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    return(nsecs);
  if (setjmp(env_alrm) == 0) {
    alarm(nsecs);
    pause();
  }
  return(alarm(0));
}

int
main(void)
{
  printf("Start\n");
  sleep2(2);
  printf("Stop\n");
  exit(0);
}
