#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLEN 4096

typedef struct Heap {
  int elements[MAXLEN];
  int size;
} Heap;

Heap *initHeap() {
  Heap *new;
  int i;

  new = malloc(sizeof(Heap));
  if (!new) return NULL;

  new->size = 0;
  for (i=0; i<MAXLEN; i++)
    new->elements[i] = 0;

  return new;
}

int left_index(int current) {
  return current * 2 + 1;
}

int right_index(int current) {
  return current * 2 + 2;
}

int parent_index(int current) {
  if (current > 0)
    return ( current - 1 ) / 2;
  else
    return 0;
}

void insertElement(Heap *heap, int value) {
  int *elements, current, parent;

  if (!heap) return;
  elements = heap->elements;
  current = heap->size++;

  /* insert element at end of heap */
  parent = parent_index(current);
  elements[current] = value;

  while (elements[parent] > value) {
    elements[current] = elements[parent];
    current = parent;
    parent = parent_index(parent);
  }

  elements[current] = value;
}

void printHeap(Heap *heap) {
  int i, size, *elements;

  if (!heap) return;
  size = heap->size;
  elements = heap->elements;

  for (i=0; i<size; i++)
    printf("%d ", elements[i]);
  printf("\n");
}

void swap(int *array, int first, int second) {
  int tmp;

  if (!array) return;
  tmp = array[first];
  array[first] = array[second];
  array[second] = tmp;
}

int extractMin(Heap *heap) {
  int size, *elements, current, min, left, right;

  if (!heap) return;
  elements = heap->elements;

  min = elements[0];
  size = --(heap->size);

  elements[0] = elements[size];
  current = 0;

  while (current < size) {
    left = left_index(current);
    right = right_index(current);

    if (right < size && elements[current] > elements[right] && elements[right] <= elements[left]) {
      swap(elements, current, right);
      current = right;
    } else if (left < size && elements[current] > elements[left]) {
      swap(elements, current, left);
      current = left;
    } else {
      break;
    }
  }

  return min;
}

int main(int argc, char **argv) {
  Heap *heap;
  int i, size;

  heap = initHeap();
  insertElement(heap, 0);
  insertElement(heap, 10);
  insertElement(heap, 4);
  insertElement(heap, 1);
  insertElement(heap, 7);
  insertElement(heap, 5);
  insertElement(heap, 17);
  printHeap(heap);

  size = heap->size;
  for(i=0; i<size; i++) {
    printf("extracted %d\n", extractMin(heap));
  }
  insertElement(heap, 24);
  printHeap(heap);

  exit(0);
}
