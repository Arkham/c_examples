#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 3

char keyMap[8][3] = {
  { 'a', 'b', 'c' },
  { 'd', 'e', 'f' },
  { 'g', 'h', 'i' },
  { 'j', 'k', 'l' },
  { 'm', 'n', 'o' },
  { 'p', 'r', 's' },
  { 't', 'u', 'v' },
  { 'w', 'x', 'y' }
};

char getCharKey(int telephoneKey, int place) {
  if (telephoneKey >= 2 && place >= 1)
    return keyMap[telephoneKey-2][place-1];
  else
    return -1;
}

void recursiveFind(int *sequence, int length, int counter, char *string) {
  int i;

  if (counter == length) {
    printf("%s\n", string);
    return;
  }

  if (sequence[counter] >= 2) {
    for (i=1; i<=3; i++) {
      string[counter] = getCharKey(sequence[counter], i);
      recursiveFind(sequence, length, counter+1, string);
    }
  } else {
    string[counter] = '0' + sequence[counter];
    recursiveFind(sequence, length, counter+1, string);
  }
}

void iterativeFind(int *sequence, int length, char *string) {
  int i;

  for (i=0; i<length; i++) {
    if (sequence[i] >= 2)
      string[i] = getCharKey(sequence[i], 1);
    else
      string[i] = '0' + sequence[i];
  }

  while(1) {
    printf("%s\n", string);

    for (i=0; i<length; i++) {
      if (string[i] == getCharKey(sequence[i], 3)) {
        string[i] = getCharKey(sequence[i], 1);
      } else if (string[i] == getCharKey(sequence[i], 2)) {
        string[i] = getCharKey(sequence[i], 3);
        break;
      } else if (string[i] == getCharKey(sequence[i], 1)) {
        string[i] = getCharKey(sequence[i], 2);
        break;
      }
    }

    if (i == length)
      return;
  }
}

int main(int argc, char **argv) {
  int sequence[LEN] = { 8, 0, 6 };
  char *string;

  string = malloc(LEN * sizeof(char));
  string[LEN] = '\0';

  printf("recursive find:\n");
  recursiveFind(sequence, LEN, 0, string);

  printf("\niterative find:\n");
  iterativeFind(sequence, LEN, string);

  return EXIT_SUCCESS;
}
