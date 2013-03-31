#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define LEN 65535

void print_array(int *array, int length) {
  int i;

  for (i=0; i<length; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");
}

int r_search(int *array, int left, int right, int haystack) {
  int pivot, p_index;

  if (left == right) return -1;

  p_index = (left+right) / 2;
  pivot = array[p_index];

  if (pivot > haystack)
    return r_search(array, left, p_index, haystack);
  else if (pivot < haystack)
    return r_search(array, p_index+1, right, haystack);

  // (pivot == haystack)
  return p_index;
}

int binary_search(int *array, int length, int haystack) {
  if (length > 1)
    return r_search(array, 0, length-1, haystack);
  else
    return 0;
}

clock_t get_ms() {
  return clock() / (CLOCKS_PER_SEC / 1000);
}

clock_t get_ns() {
  return clock() / (CLOCKS_PER_SEC / 1000000);
}

void measureSearch(int *array, int length, int haystack, int (*search_function) (int*, int, int)) {
  int index;
  clock_t start,end;
  double period;

  start = get_ns();
  index = (*search_function)(array, length, haystack);
  end = get_ns();
  period = difftime(end, start);

  if (index >= 0)
    printf ("Haystack %d found at index %d\n", haystack, index);
  else
    printf("Haystack %d not found\n", haystack);

  printf ("time: %.lf ns\n", period);
}

int iterative_search(int *array, int length, int haystack) {
  int i;

  for (i=0; i<length; i++)
    if (array[i] == haystack)
      return i;

  return -1;
}

int main(int argc, char **argv) {
  int i, array[LEN];

  for (i=0; i<LEN; i++) {
    array[i] = i*2;
  }

  measureSearch(array, LEN, -1, &binary_search);
  measureSearch(array, LEN, 20, &binary_search);
  measureSearch(array, LEN, 12000, &binary_search);
  measureSearch(array, LEN, 42000, &binary_search);

  measureSearch(array, LEN, -1, &iterative_search);
  measureSearch(array, LEN, 20, &iterative_search);
  measureSearch(array, LEN, 12000, &iterative_search);
  measureSearch(array, LEN, 42000, &iterative_search);

  return EXIT_SUCCESS;
}
