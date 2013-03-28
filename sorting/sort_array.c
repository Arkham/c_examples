#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LEN 6

void swap(int *array, int source, int dest) {
  int temp;

  temp = array[source];
  array[source] = array[dest];
  array[dest] = temp;
}

void selection_sort(int *array, int length) {
  int i, j, min;

  for (i=0; i<length; i++) {
    min = i;

    for (j=i; j<length; j++) {
      if (array[j] < array[min])
        min = j;
    }

    if (min != i)
      swap(array, min, i);
  }
}

void bubble_sort(int *array, int length) {
  int i, j;

  for (i=0; i<length; i++) {
    for (j=i; j<length; j++) {
      if (array[j] < array[i]) {
        swap(array, i, j);
      }
    }
  }
}

void insertion_sort(int *array, int length) {
  int i, j, value;

  if (length < 2) return;

  for (i=1; i<length; i++) {
    value = array[i];
    j = i - 1;

    while (j >= 0 &&  value < array[j]) {
      array[j+1] = array[j];
      j--;
    }
    array[j+1] = value;
  }
}

void print(int *array, int length) {
  int i;

  for (i = 0; i < length; ++i) {
    printf("%d ", array[i]);
  }
}

int main(int argc, char *argv[]) {
  int array[LEN] = { 1, 4, 5, 2, 10, 3 };

  /* selection_sort(array, LEN); */
  /* bubble_sort(array, LEN); */
  insertion_sort(array, LEN);
  print(array, LEN);

  return EXIT_SUCCESS;
}
