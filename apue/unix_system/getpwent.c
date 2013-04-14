#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct passwd *
my_getpwnam(const char *name)
{
  struct passwd *ptr = NULL;

  setpwent();

  while ( (ptr = getpwent()) != NULL)
    if (strcmp(ptr->pw_name, name) == 0)
      break;

  endpwent();

  return ptr;
}

void find_user(const char *name) {
  struct passwd *result;

  if ( (result = my_getpwnam(name)) != NULL)
    printf("user %s found with id %d\n", result->pw_name, result->pw_uid);
  else
    printf("user %s not found\n", name);
}

int
main(void) {

  find_user("root");
  find_user("nobody");
  find_user("jesus");

  exit(0);
}
