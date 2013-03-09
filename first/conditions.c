#include <stdio.h>
#include <stdlib.h>

void print_largest(int value) {
  printf("The largest number is %d\n", value);
}

int main(int argc, char *argv[]) {
  int first, second;

  printf("Insert two numbers brah: ");
  scanf("%d %d", &first, &second);

  if (first > second) {
    print_largest(first);
  } else if (second > first) {
    print_largest(second);
  } else {
    printf("Surprise! The numbers are equal!\n");
  }

  return EXIT_SUCCESS;
}
