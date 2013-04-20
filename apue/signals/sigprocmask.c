#include "../apue.h"
#include <errno.h>

void
pr_mask(const char *str)
{
  sigset_t sigset;
  int errno_save;

  errno_save = errno;
  if (sigprocmask(0, NULL, &sigset) < 0)
    err_sys("sigprocmask error");

  printf("%s: ", str);

  if (sigismember(&sigset, SIGINT))  printf("SIGINT ");
  if (sigismember(&sigset, SIGQUIT))  printf("SIGQUIT ");
  if (sigismember(&sigset, SIGUSR1))  printf("SIGUSR1 ");
  if (sigismember(&sigset, SIGALRM))  printf("SIGALRM ");

  printf("\n");
  errno = errno_save;
}

static void
sig_quit(int signo)
{
  printf("Caught SIGQUIT\n");
  if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    err_sys("can't reset SIGQUIT");
}

int
main(void)
{
  sigset_t newmask, oldmask, pendmask;

  if (signal(SIGQUIT, sig_quit) == SIG_ERR)
    err_sys("can't catch SIGQUIT");

  sigemptyset(&newmask);
  sigaddset(&newmask, SIGQUIT);

  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    err_sys("SIG_BLOCK error");
  pr_mask("main");

  sleep(5);

  if (sigpending(&pendmask) < 0)
    err_sys("sigpending error");
  if (sigismember(&pendmask, SIGQUIT))
    printf("\nSIGQUIT pending\n");

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    err_sys("SIG_SETMASK error");
  printf("SIGQUIT unblocked\n");
  pr_mask("main");

  sleep(5);
  exit(0);
}
