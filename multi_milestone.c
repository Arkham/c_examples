#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEPS 4
#define STEP_RANGE 10
#define MAXLEN 100

struct analysis {
  int count;
  int total;
  int sum_squared;
  int limits[STEPS];
  int counters[STEPS+1];
};

typedef struct analysis Analysis;

void initialize(Analysis *a) {
  int i,
      *limits = a->limits,
      *counters = a->counters;

  a->count = 0;
  a->total = 0;
  a->sum_squared = 0;

  for (i=0; i<STEPS; i++) {
    limits[i] = (i+1) * STEP_RANGE;
    counters[i] = 0;
  }
  counters[i] = 0;
}

float get_mean(Analysis *a) {
  return ((float) a->total) / a->count;
}

float get_standard_deviation(Analysis *a) {
  return sqrt((a->sum_squared - (a->total * a->total)/a->count)/(a->count - 1));
}

void text_results(Analysis *a) {
  int i,
      *limits = a->limits,
      *counters = a->counters;

  printf("\n\n");
  for (i=0; i<STEPS; i++) {
    printf("Numbers lower than %d: %d\n", limits[i], counters[i]);
  }
  printf("Number over %d: %d\n", limits[STEPS-1], counters[STEPS]);

  printf("Numbers count: %d\n", a->count);
  printf("Numbers total: %d\n", a->total);
  printf("Numbers mean: %.2f\n", get_mean(a));
  printf("Numbers standard deviation: %.2f\n", get_standard_deviation(a));
}

void histogram_results(Analysis *a) {
  int i, j, max=0,
      *counters = a->counters;

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
  printf("=\n");

  for (j=0; j<=STEPS; j++) printf("%4d", j*STEP_RANGE);
  printf("+\n");
}

void read_values(Analysis *a) {
  int i, current,
      *limits = a->limits,
      *counters = a->counters;

  printf("Type in some numbers (Ctrl-d to end)\n");

  while (scanf("%d\n", &current) != EOF) {
    a->count++;
    a->total += current;
    a->sum_squared += current * current;

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
}

int main(int argc, char const *argv[])
{
  Analysis *a = malloc(sizeof(Analysis));

  initialize(a);
  read_values(a);

  text_results(a);
  /* histogram_results(a); */

  return 0;
}
