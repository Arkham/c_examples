#include "../apue.h"

static void
sig_int(int signo)
{
  printf("caught SIGINT\n");
}

static void
sig_chld(int signo)
{
  printf("caught SIGCHLD\n");
}

int
my_system(const char *cmd)
{
  pid_t pid;
  int status;

  if (cmd == NULL)
    return(1);

  if ( (pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {
    /* child */
    execl("/bin/sh", "sh", "-c", cmd, (char *) 0);
    _exit(127);
  }

  while (waitpid(pid, &status, 0) < 0) {
    if (errno != EINTR) {
      status = -1;
      break;
    }
  }

  return(status);
}

int
main(void)
{
  if (signal(SIGINT, sig_int) == SIG_ERR)
    err_sys("can't catch SIGINT");
  if (signal(SIGCHLD, sig_chld) == SIG_ERR)
    err_sys("can't catch SIGCHLD");

  if (my_system("/bin/ed") < 0)
    err_sys("system error");
  exit(0);
}
