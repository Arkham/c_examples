#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   hat

   hat
   hta

   aht
   ath

   tha
   tah
*/

void r_permutation(char *string, int length, int *mask, int counter) {
  int i;

  if (counter == length) {
    for (i=0; i<length; i++)
      printf("%c", string[mask[i]]);
    printf("\n");
  }

  for (i=0; i<length; i++) {
    if (mask[i] < 0) {
      mask[i] = counter;
      r_permutation(string, length, mask, counter + 1);
      mask[i] = -1;
    }
  }
}

void permutation(char *string) {
  int len, i;
  int *mask;

  len = strlen(string);
  mask = malloc(len * sizeof(int));
  for (i=0; i<len; i++)
    mask[i] = -1;

  r_permutation(string, len, mask, 0);
}

int main(int argc, char **argv) {
  char string[] = "hat";

  permutation(string);

  return EXIT_SUCCESS;
}
