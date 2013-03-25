#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  struct Node *left;
  struct Node *right;
  int value;
} Node;

Node *createNode(value) {
  Node *new;

  new = malloc(sizeof(Node));
  if (!new) return NULL;

  new->left = NULL;
  new->right = NULL;
  new->value = value;
  return new;
}

void insert(Node **root, int value) {
  Node *new, *current;

  if (!root) return;

  current = *root;

  if (current) {
    if (value < current->value)
      insert(&(current->left), value);
    else if (value > current->value)
      insert(&(current->right), value);
  } else {
    new = createNode(value);
    *root = new;
  }
}

void traverse(Node *root) {
  if (!root) return;

  traverse(root->left);
  printf("Value: %d\n", root->value);
  traverse(root->right);
}

int main(int argc, char *argv[]) {
  Node *root;

  insert(&root, 5);
  insert(&root, 1);
  insert(&root, 10);
  insert(&root, 6);
  insert(&root, 12);
  traverse(root);

  return EXIT_SUCCESS;
}
