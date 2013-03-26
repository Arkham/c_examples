#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char first_non_repeated(char *string) {
  char map[255] = { 0 };
  int length, i, value;

  length = strlen(string);
  for (i=0; i<length; i++) {
    value = string[i] - '0';
    map[value]++;
  }

  for (i=0; i<length; i++) {
    value = string[i] - '0';
    if (map[value] == 1)
      return string[i];
  }

  return 0;
}

int main(int argc, char *argv[]) {
  char first[] = "total";
  char second[] = "teeter";

  printf("First non repeated char is %c\n", first_non_repeated(first));
  printf("First non repeated char is %c\n", first_non_repeated(second));

  return EXIT_SUCCESS;
}
