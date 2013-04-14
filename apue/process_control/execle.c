#include "../apue.h"
#include <sys/wait.h>

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int
main(void)
{
  pid_t pid;

  if ( (pid = fork()) < 0 )
    err_sys("fork error");
  else if (pid == 0) {
    if (execle("./bin/echoall", "echoall", "first", "second", NULL, env_init) < 0)
      err_sys("execle error");
  }

  if (waitpid(pid, NULL, 0) != pid)
    err_sys("waitpid error");

  if ( (pid = fork()) < 0)
    err_sys("fork error");
  else if (pid == 0) {
    if (execlp("echoall", "echoall", "first", NULL) < 0)
      err_sys("execlp error");
  }

  if (waitpid(pid, NULL, 0) != pid)
    err_sys("waitpid error");

  exit(0);
}
