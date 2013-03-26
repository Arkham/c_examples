#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  struct Node *left;
  struct Node *right;
  int value;
  int visited;
} Node;

Node *createNode(int value) {
  Node *new;

  new = malloc(sizeof(Node));
  if (!new) return NULL;

  new->left = NULL;
  new->right = NULL;
  new->visited = -1;
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

  new = createNode(value);
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

int countNodes(Node *root) {
  if (!root) return 0;
  return 1 + countNodes(root->left) + countNodes(root->right);
}

typedef struct Element {
  struct Element *next;
  Node *elem;
} Element;

Element * createElement(Node *elem) {
  Element *new;

  new = malloc(sizeof(Element));
  if (!new) return NULL;

  new->next = NULL;
  new->elem = elem;
  return new;
}

bool insertQueue(Element **queue, Node *elem) {
  Element *current, *new;

  if (!queue) return false;

  new = createElement(elem);
  if (!new) return false;

  if (!*queue) {
    *queue = new;
    return true;
  }

  current = *queue;
  while (current->next) { current = current->next; }
  current->next = new;

  return true;
}

Element * extractFirst(Element **queue) {
  Element *current;

  if (!queue || !*queue) return NULL;

  current = *queue;
  *queue = (*queue)->next;
  return current;
}

void BFS(Node *root) {
  Element *queue = NULL, *current_el;
  Node *node;

  insertQueue(&queue, root);

  while (queue) {
    current_el = extractFirst(&queue);
    if (!current_el) return;

    node = current_el->elem;
    if (!node) return;

    node->visited++;
    printNode(node);
    printf("Node distance: %d\n", node->visited);

    if (node->left && node->left->visited < 0) {
      node->left->visited = node->visited;
      insertQueue(&queue, node->left);
    }

    if (node->right && node->right->visited < 0) {
      node->right->visited = node->visited;
      insertQueue(&queue, node->right);
    }
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

  printf("\nBFS!\n\n");
  BFS(tree);

  return EXIT_SUCCESS;
}
