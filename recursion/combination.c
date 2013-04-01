#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   hat ->

   h
   a
   t
   ha
   ht
   at
   hat
*/

void r_combine(char *string, int length, int *mask, int start, int counter, int limit) {
  int i, j;

  if (counter == limit) {

    /* printf("mask: "); */
    /* for (i=0; i<length; i++) */
    /*   printf("%d ", mask[i]); */
    /* printf("\n"); */

    for (i=0; i<limit; i++)
      for (j=0; j<length; j++)
        if (mask[j] == i)
          printf("%c", string[j]);

    printf("\n");
  }

  for (i=start; i<length; i++) {
    if (mask[i] < 0) {
      mask[i] = counter;
      r_combine(string, length, mask, i+1, counter+1, limit);
      mask[i] = -1;
    }
  }
}

void combination(char *string) {
  int length, i, j, *mask;

  length = strlen(string);
  mask = malloc(length * sizeof(int));

  for (i=0; i<length; i++) {
    for (j=0; j<length; j++)
      mask[j] = -1;
    r_combine(string, length, mask, 0, 0, i+1);
  }
}

int main(int argc, char **argv) {
  char string[] = "fof";

  combination(string);

  return EXIT_SUCCESS;
}
