#include "../apue.h"
#include <pwd.h>

static void
my_alarm(int signo)
{
  struct passwd *rootptr;

  printf("in signal handler\n");
  if ( (rootptr = getpwnam("root")) == NULL)
    err_sys("getpwnam(root) error");

  alarm(1);
}

int
main(void)
{
  struct passwd *ptr;

  printf("in main\n");
  signal(SIGALRM, my_alarm);
  alarm(1);

  while (1) {
    if ( (ptr = getpwnam("nobody")) == NULL)
      err_sys("getpwnam(nobody) error");
    if (strcmp(ptr->pw_name, "nobody") != 0)
      printf("return value corrupted, pw_name = %s\n", ptr->pw_name);
  }

  printf("exit main\n");
  exit(0);
}
