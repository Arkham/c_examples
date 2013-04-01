#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXLEN 128

typedef struct User {
  char name[MAXLEN];
  char surname[MAXLEN];
  int age;
} User;

User *createUser(char *name, char *surname, int age) {
  User *new;

  new = malloc(sizeof(User));
  if (!new) return NULL;

  strlcpy(new->name, name, MAXLEN);
  strlcpy(new->surname, surname, MAXLEN);
  new->age = age;

  return new;
}

int compareUser(User **first, User **second) {
  int value;
  User *first_user = *first;
  User *second_user = *second;

  value = strcmp(first_user->surname, second_user->surname);
  if (value != 0) {
    return value;
  }

  value = strcmp(first_user->name, second_user->name);
  if (value != 0) {
    return value;
  }

  if (first_user->age > second_user->age)
    return 1;
  else if (first_user->age < second_user->age)
    return -1;

  return 0;
}

typedef int (*compfn)(const void*, const void*);

int main(int argc, char **argv) {
  int i;
  User *list[4];

  list[0] = createUser("Mario", "Verdi", 13);
  list[1] = createUser("Silvia", "Rossi", 23);
  list[2] = createUser("Mario", "Rossi", 27);
  list[3] = createUser("Mario", "Rossi", 22);

  qsort((void *) &list, 4, sizeof(User *), (compfn) &compareUser);

  for (i=0; i<4; i++) {
    printf("User: %s %s, age %d\n", list[i]->surname, list[i]->name, list[i]->age);
  }

  return EXIT_SUCCESS;
}
