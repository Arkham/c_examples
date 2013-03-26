#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int char_to_i(char c) {
  return c - '0';
}

char * remove_chars(char *string, char *remove) {
  char map[255] = { 0 };
  int length, i, value, written = 0;

  length = strlen(remove);
  for (i=0; i<length; i++) {
    value = char_to_i(remove[i]);
    map[value]++;
  }

  length = strlen(string);
  for (i=0; i<length; i++) {
    value = char_to_i(string[i]);
    if (map[value])
      continue;
    else
      string[written++] = string[i];
  }

  string[written] = '\0';
  return string;
}

int main(int argc, char *argv[]) {
  char original[] = "Battle of the Vowels: Hawaii vs. Grozny";
  char remove[] = "aeiou";

  printf("original: %s\n", original);
  printf("remove: %s\n", remove);
  printf("result: %s\n", remove_chars(original, remove));
  return EXIT_SUCCESS;
}

