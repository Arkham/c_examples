#include "../apue.h"

static void
my_alarm(int signo)
{
  printf("signal handler: ");
  signal(SIGALRM, my_alarm);
  printf("starting alarm, previous left = %d\n", alarm(1));
}

int
main(void)
{
  signal(SIGALRM, my_alarm);

  printf("main: starting alarm, previous left = %d\n", alarm(3));
  sleep(2);
  printf("main: starting alarm, previous left = %d\n", alarm(3));

  while (1)
    pause();

  exit(0);
}
