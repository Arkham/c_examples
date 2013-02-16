#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int next, total;

  scanf("%d", &next);
  total = 0;

  while(next != -1) {
    total += next;
    scanf("%d", &next);
  }

  printf("The total is %d\n", total);
  return EXIT_SUCCESS;
}
