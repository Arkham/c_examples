#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 65536

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
  printf("\n");
}

void r_quicksort(int *array, int start, int end) {
  int pivot, start_index, end_index;

  start_index = start;
  end_index = end;
  pivot = array[(start + end)/2];

  while (start <= end) {
    while (array[start] < pivot) {
      start++;
    }
    while (array[end] > pivot) {
      end--;
    }
    if (start <= end) {
      swap(array, start++, end--);
    }
  }

  if (start_index < end)
    r_quicksort(array, start_index, end);

  if (start < end_index)
    r_quicksort(array, start, end_index);
}

void quicksort(int *array, int length) {
  r_quicksort(array, 0, length - 1);
}

void initRand(int *array, int length) {
  int i;
  for (i=0; i<length; i++)
    array[i] = rand() % MAX;
}

clock_t get_clock() {
  return clock() / (CLOCKS_PER_SEC / 1000);
}

void measureSort(const char *name, void (*sortFunction)(int *, int)) {
  clock_t start,end;
  double period;
  int array[MAX];

  initRand(array, MAX);

  start = get_clock();
  (*sortFunction)(array, MAX);
  end = get_clock();

  period = difftime(end, start);
  printf ("%s time: %.lf ms\n", name, period);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  measureSort("selection", &selection_sort);
  measureSort("bubble", &bubble_sort);
  measureSort("insertion", &selection_sort);
  measureSort("quicksort", &quicksort);

  return EXIT_SUCCESS;
}
