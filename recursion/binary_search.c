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
  int pivot, middle;

  if (left >= right) return -1;

  middle = (left+right) / 2;
  pivot = array[middle];

  if (pivot == haystack)
    return middle;
  else if (pivot > haystack)
    return r_search(array, left, middle, haystack);
  else if (pivot < haystack)
    return r_search(array, middle+1, right, haystack);

  return -1;
}

int binary_search(int *array, int length, int haystack) {
  return r_search(array, 0, length-1, haystack);
}

int iter_binary_search(int *array, int length, int haystack) {
  int left, right, middle, pivot;

  left = 0;
  right = length - 1;

  while (left < right) {
    middle = (left + right) / 2;
    pivot = array[middle];

    if (pivot == haystack)
      return middle;

    if (pivot > haystack)
      right = middle;
    else
      left = middle + 1;
  }

  return -1;
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

int selection_search(int *array, int length, int haystack) {
  int i, value;

  for (i=0; i<length; i++) {
    value = array[i];
    if (value == haystack) 
      return i;

    if (value > haystack)
      break;
  }

  return -1;
}

int main(int argc, char **argv) {
  int i, array[LEN];

  for (i=0; i<LEN; i++) {
    array[i] = i*2;
  }
  printf("%d elements\n", LEN);

  printf("\nSelection:\n");
  measureSearch(array, LEN, -1, &selection_search);
  measureSearch(array, LEN, 20, &selection_search);
  measureSearch(array, LEN, 30000, &selection_search);
  measureSearch(array, LEN, 60000, &selection_search);
  measureSearch(array, LEN, 70000, &selection_search);

  printf("\nRecursive binary search:\n");
  measureSearch(array, LEN, -1, &binary_search);
  measureSearch(array, LEN, 20, &binary_search);
  measureSearch(array, LEN, 30000, &binary_search);
  measureSearch(array, LEN, 60000, &binary_search);
  measureSearch(array, LEN, 70000, &binary_search);

  printf("\nIterative binary search:\n");
  measureSearch(array, LEN, -1, &iter_binary_search);
  measureSearch(array, LEN, 20, &iter_binary_search);
  measureSearch(array, LEN, 30000, &iter_binary_search);
  measureSearch(array, LEN, 60000, &iter_binary_search);
  measureSearch(array, LEN, 70000, &iter_binary_search);

  return EXIT_SUCCESS;
}
