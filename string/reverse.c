#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isSeparator(char c) {
  return c == ' ';
}

void copyReverseWord(char *source, char *dest, int length, int start, int end) {
  int i, rev_i;

  rev_i = length - 1 - end;
  for (i=start; i<=end; i++) {
    dest[rev_i++] = source[i];
  }
}

char *reverse(char *string) {
  int length, i, rev_i, start_word;
  char *result;
  bool in_word = false;

  length = strlen(string);
  result = malloc(length * sizeof(char));

  for (i=0; i<length; i++) {
    rev_i = length - 1 - i;

    if (isSeparator(string[i])) {
      if (in_word) {
        copyReverseWord(string, result, length, start_word, i-1);
        in_word = false;
      }
      result[rev_i] = string[i];
    } else {
      if (!in_word) {
        in_word = true;
        start_word = i;
      }
    }
  }

  if (in_word)
    copyReverseWord(string, result, length, start_word, i-1);

  return result;
}

void swapStringRange(char *string, int start, int end) {
  char temp;

  while(start < end)
  {
    temp = string[start];
    string[start] = string[end];
    string[end] = temp;

    start++;
    end--;
  }
}

char *better_reverse(char *string) {
  int length, i=0, start_index;

  length = strlen(string);
  swapStringRange(string, 0, length-1);

  while (i<length) {
    if (string[i] != ' ') {
      start_index = i;
      while (string[i] != ' ' && i<length) {
        i++;
      }
      swapStringRange(string, start_index, i-1);
    } else {
      i++;
    }
  }

  return string;
}

int main(int argc, char *argv[]) {
  char string[] = "Do or do not, there is no try";

  printf("Original: %s\n", string);
  printf("reverse: %s\n", reverse(string));

  printf("\nOriginal: %s\n", string);
  printf("reverse: %s\n", better_reverse(string));

  return EXIT_SUCCESS;
}
