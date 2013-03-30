#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int string_to_int(char *string) {
  int length, i = 0, total = 0, value;
  bool negative = false;

  length = strlen(string);
  if (string[0] == '-') {
    negative = true;
    i = 1;
  }

  for (; i<length; i++) {
    value = string[i] - '0';
    total *= 10;
    total += value;
  }

  return negative ? -1 * total : total;
}

int main(int argc, char *argv[]) {
  char string[] = "-123";
  int result;

  result = string_to_int(string);

  if (result == -123) {
    printf("Conversion successful!\n");
  } else {
    printf("Uh, something's wrong, I got %d\n", result);
  }

  return EXIT_SUCCESS;
}
