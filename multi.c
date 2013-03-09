#include <stdio.h>
#include <stdlib.h>

#define STEPS 4
#define STEP_RANGE 10

int main(int argc, char const *argv[])
{
  int limits[STEPS], counters[STEPS+1] = { 0 };
  int current, i, j, max=0;

  for (i=0; i<STEPS; i++) {
    limits[i] = (i+1) * STEP_RANGE;
  }

  printf("Type in some numbers (Ctrl-d to end)\n");

  while (scanf("%d\n", &current) != EOF) {
    if (current > limits[STEPS-1]) {
      counters[STEPS]++;
      continue;
    }

    for (i=0; i<STEPS; i++) {
      if (current < limits[i]) {
        counters[i]++;
        break;
      }
    }
  }

  printf("\n\n");
  for (i=0; i<STEPS; i++) {
    printf("Numbers lower than %d: %d\n", limits[i], counters[i]);
  }
  printf("Number over %d: %d\n\n", limits[STEPS-1], counters[STEPS]);

  for (i=0; i<=STEPS; i++) {
    if (counters[i] > max)
      max = counters[i];
  }

  for (i=max; i>=1; i--) {
    for (j=0; j<=STEPS; j++) {
      if (counters[j] >= i)
        printf("  **");
      else
        printf("    ");
    }
    printf("\n");
  }

  for (j=0; j<=STEPS; j++) printf("====");
  printf("\n");

  for (j=0; j<=STEPS; j++) printf("%4d", j*STEP_RANGE);
  printf("+\n");

  return 0;
}
