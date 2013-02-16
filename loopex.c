#include <stdio.h>
#include <stdlib.h>

void print_stars(count) {
  int i;
  for (i=0; i<count; i++) {
    printf("*");
  }
  printf("\n");
}

int main() {
  int next, total_to_10, total_to_20, total_to_30, total_over_30;

  scanf("%d", &next);

  total_to_10 = 0;
  total_to_20 = 0;
  total_to_30 = 0;
  total_over_30 = 0;

  while (next != -1) {
    if (next <= 10)
      total_to_10 += 1;
    else if (next <= 20)
      total_to_20 += 1;
    else if (next <= 30)
      total_to_30 += 1;
    else
      total_over_30 += 1;

    scanf("%d", &next);
  }

  printf("The total under 11 is: %d\n", total_to_10);
  printf("The total under 21 is: %d\n", total_to_20);
  printf("The total under 31 is: %d\n", total_to_30);
  printf("The total over 30 is:  %d\n", total_over_30);

  printf("the total under 11: ");
  print_stars(total_to_10);
  printf("the total under 21: ");
  print_stars(total_to_20);
  printf("the total under 31: ");
  print_stars(total_to_30);
  printf("the total over 30:  ");
  print_stars(total_over_30);

  return EXIT_SUCCESS;
}
