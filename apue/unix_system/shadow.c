#include "../apue.h"
#include <pwd.h>

int
main(void)
{
  struct passwd *ptr;

  if ( (ptr = getpwnam("arkham")) == NULL)
    err_sys("getpwnam failed");

  printf("user %s, id %d\n", ptr->pw_name, ptr->pw_uid);
  printf("encrypted password is %s\n", ptr->pw_passwd);

  exit(0);
}
