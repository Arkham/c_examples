#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("int: %lu\n", sizeof(int));
  printf("long int: %lu\n", sizeof(long int));
  printf("short int: %lu\n", sizeof(short int));
  printf("float: %lu\n", sizeof(float));
  printf("double: %lu\n", sizeof(double));
  printf("char: %lu\n", sizeof(char));

  return EXIT_SUCCESS;
}
