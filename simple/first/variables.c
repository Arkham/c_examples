#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int char_value;

  char_value = getchar();
  if (char_value == 'a')
    printf("%s Success\n", argv[1]);
  else
    printf("Failure\n");

  return EXIT_SUCCESS;
}
