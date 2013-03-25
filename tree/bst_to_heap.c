#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  struct Node *left;
  struct Node *right;
  int value;
} Node;

Node *createElement(int value) {
  Node *new;

  new = malloc(sizeof(Node));
  if (!new) return NULL;

  new->left = NULL;
  new->right = NULL;
  new->value = value;
  return new;
}

bool insert(Node **root, Node *elem) {
  Node *current;

  if (!root) return false;
  current = *root;

  if (!current) {
    *root = elem;
    return true;
  }

  if (current->value > elem->value)
    return insert(&(current->left), elem);
  else
    return insert(&(current->right), elem);

  return false;
}

void insertValue(Node **root, int value) {
  Node *new;

  if (!root) return;

  new = createElement(value);
  if (!new) return;

  insert(root, new);
}

void printNode(Node *root) {
  printf("Node value: %d\n", root->value);
}

void traverseTree(Node *root) {
  if (!root) return;

  printNode(root);
  traverseTree(root->left);
  traverseTree(root->right);
}

int count(Node *root) {
  if (!root) return 0;
  return 1 + count(root->left) + count(root->right);
}

void loadHeap(Node **heap, Node *root, int *position) {
  if (!root) return;

  heap[*position] = root;

  if (root->left) {
    (*position)++;
    loadHeap(heap, root->left, position);
  }

  if (root->right) {
    (*position)++;
    loadHeap(heap, root->right, position);
  }
}

void sortArray(Node **array, int length) {
  int i, j;
  Node *temp;

  for (i=0; i<length; i++)
    for (j=i+1; j<length; j++)
      if (array[i]->value > array[j]->value) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
      }
}

Node **heapifyTree(Node *root) {
  int tree_size, i = 0;
  Node **heap;

  tree_size = count(root);
  heap = malloc(tree_size * sizeof(Node *));
  loadHeap(heap, root, &i);

  for (i = 0; i < tree_size; i++) {
    printNode(heap[i]);
  }

  printf("\nSorting array..\n\n");
  sortArray(heap, tree_size);

  for (i = 0; i < tree_size; i++) {
    printNode(heap[i]);
  }

  return heap;
}

void treefyHeap(Node **heap, int length, Node **root) {
  int i, left_i, right_i;

  if (length <= 0) return;
  *root = heap[0];

  for (i=0; i<length; i++) {
    left_i = 2 * i + 1;
    right_i = left_i + 1;

    heap[i]->left = (left_i < length) ? heap[left_i] : NULL;
    heap[i]->right = (right_i < length) ? heap[right_i] : NULL;
  }
}

int main(int argc, char *argv[]) {
  Node *tree;
  Node **heap;
  int length;

  insertValue(&tree, 5);
  insertValue(&tree, 3);
  insertValue(&tree, 7);
  insertValue(&tree, 4);
  insertValue(&tree, 2);
  insertValue(&tree, 1);

  traverseTree(tree);

  length = count(tree);
  printf("\nTree has %d elements\n", length);

  printf("\nHeap!\n\n");
  heap = heapifyTree(tree);

  printf("\nTree!\n\n");
  treefyHeap(heap, length, &tree);
  traverseTree(tree);

  return EXIT_SUCCESS;
}
