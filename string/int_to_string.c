#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void reverseArray(char *array, int length) {
  int start, end, temp;

  start = 0;
  end = length - 1;

  while (start < end) {
    temp = array[start];
    array[start] = array[end];
    array[end] = temp;
    start++;
    end--;
  }
}

void int_to_string(int value, char *result) {
  int k = 0, rest;
  bool negative = false;

  if (value < 0) {
    negative = true;
    value *= -1;
  }

  while (value > 0) {
    rest = value % 10;
    value /= 10;
    result[k++] = rest + '0';
  }

  if (negative) {
    result[k++] = '-';
  }

  reverseArray(result, k);
  result[k] = '\0';
}

int main(int argc, char *argv[]) {
  int value = -123;
  char result[10] = { 0 };

  int_to_string(value, result);

  if (strcmp(result, "-123") == 0) {
    printf("Conversion successful!\n");
  } else {
    printf("Uh, something's wrong, I got '%s'\n", result);
  }

  return EXIT_SUCCESS;
}
