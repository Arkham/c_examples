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

void merge(int *array, int *work, int left, int middle, int right) {
  int i, j, k;

  i = left;
  j = middle+1;
  k = left;

  while (i <= middle && j <= right) {
    if (array[i] <= array[j]) {
      work[k] = array[i];
      k++;
      i++;
    } else {
      work[k] = array[j];
      k++;
      j++;
    }
  }

  while (i <= middle) {
    work[k] = array[i];
    k++;
    i++;
  }

  while (j <= right) {
    work[k] = array[j];
    k++;
    j++;
  }

  for (i=left; i<=right; i++) {
    array[i] = work[i];
  }
}

void r_mergesort(int *array, int *work, int left, int right) {
  int middle;

  if (left < right) {
    middle = (left + right) / 2;

    r_mergesort(array, work, left, middle);
    r_mergesort(array, work, middle+1, right);
    merge(array, work, left, middle, right);
  }
}

void merge_sort(int *array, int length) {
  int *work;

  work = malloc(length * sizeof(int));
  r_mergesort(array, work, 0, length - 1);
  free(work);
}

void initRand(int *array, int length) {
  int i;
  for (i=0; i<length; i++)
    array[i] = rand() % MAX;
}

clock_t get_clock() {
  return clock() / (CLOCKS_PER_SEC / 1000);
}

void checkSort(int *array, int length) {
  int i;
  for(i=0; i<length-1; i++) {
    if (array[i] > array[i+1]) {
      printf("Unsorted.. ");
      fflush(stdout);
      return;
    }
  }

  printf("Sorted! ");
  fflush(stdout);
}

void measureSort(const char *name, void (*sortFunction)(int *, int)) {
  clock_t start,end;
  double period;
  int array[MAX];

  initRand(array, MAX);
  checkSort(array, MAX);

  start = get_clock();
  (*sortFunction)(array, MAX);
  end = get_clock();

  period = difftime(end, start);

  checkSort(array, MAX);
  printf ("%s time: %.lf ms\n", name, period);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  measureSort("Bubble", &bubble_sort);
  measureSort("Selection", &selection_sort);
  measureSort("Insertion", &selection_sort);
  measureSort("Merge", &merge_sort);
  measureSort("Quick", &quicksort);

  return EXIT_SUCCESS;
}
