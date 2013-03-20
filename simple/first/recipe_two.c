#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (getchar() == 'a')
    printf("Success\n");
  else
    printf("Failure\n");

  return EXIT_SUCCESS;
}
