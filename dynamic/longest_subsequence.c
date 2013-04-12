#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

unsigned long
longest_increasing_subsequence(const int *a, unsigned long n, int *output) {
  struct lis_data {
    unsigned long length;
    unsigned long prev;
  } *table;

  unsigned long best;
  unsigned long scan;

  unsigned long i;
  unsigned long j;
  unsigned long best_length;

  if (n == 0) return 0;

  table = malloc(sizeof(*table) * n);

  for (i=0; i<n; i++) {
    table[i].length = 1;
    table[i].prev = n;

    for (j=0; j<i; j++) {
      if (a[j] < a[i] && table[j].length+1 > table[i].length) {
        table[i].length = table[j].length+1;
        table[i].prev = j;
      }
    }
  }

  best = 0;

  for (i=1; i<n; i++) {
    if (table[i].length > table[best].length) {
      best = i;
    }
  }

  best_length = table[best].length;

  for (i=0; i<n; i++)
    printf("%ld ", table[i].length);
  printf("\n");

  if (output) {
    scan = best;
    for (i=0; i<best_length; i++) {
      if (scan >= 0 && scan < n) {
        output[best_length - i - 1] = a[scan];
        scan = table[scan].prev;
      }
    }
  }

  free(table);
  return best_length;
}

int main(int argc, char *argv[]) {
  int i, result;
  int output[9];
  int input[9] = { 1,2,1,5,2,4,6,7,3 };

  for (i=0; i<9; i++)
    printf("%d ", input[i]);
  printf("\n");

  result = longest_increasing_subsequence(input, 9, output);
  printf("result: %d\n", result);
  for (i=0; i<result; i++)
    printf("%d ", output[i]);

  return EXIT_SUCCESS;
}
