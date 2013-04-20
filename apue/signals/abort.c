#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t sigflag;

void
my_abort(void)
{
  sigset_t mask;
  struct sigaction action;

  sigaction(SIGABRT, NULL, &action);
  if (action.sa_handler == SIG_IGN) {
    action.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &action, NULL);
  }

  if (action.sa_handler == SIG_DFL)
    fflush(NULL);

  sigfillset(&mask);
  sigdelset(&mask, SIGABRT);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  kill(getpid(), SIGABRT);

  printf("process caught SIGABRT\n");
  fflush(NULL);
  action.sa_handler = SIG_DFL;
  sigaction(SIGABRT, &action, NULL);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  kill(getpid(), SIGABRT);

  exit(1);
}

static void
sig_abrt(int signo)
{
  printf("inside SIGABRT handler\n");
}

static void
sig_int(int signo)
{
  printf("inside SIGINT handler\n");

  if (sigflag++ > 5)
    my_abort();
}

void
err_sys(char *msg)
{
  printf("%s\n", msg);
  exit(1);
}

int
main(void)
{
  sigset_t newmask, oldmask, zeromask;

  if (signal(SIGABRT, sig_abrt) < 0)
    err_sys("can't catch SIGABRT");
  if (signal(SIGINT, sig_int) < 0)
    err_sys("can't catch SIGINT");

  sigflag = 0;
  sigemptyset(&zeromask);
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGABRT);
  sigaddset(&newmask, SIGINT);
  sigprocmask(SIG_BLOCK, &newmask, &oldmask);

  printf("SIGINT and SIGABRT blocked\n");
  sleep(5);

  while (1)
    sigsuspend(&zeromask);

  sigprocmask(SIG_SETMASK, &oldmask, NULL);
  exit(0);
}
