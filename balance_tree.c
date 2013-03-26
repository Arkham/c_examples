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

void rotateRight(Node **root) {
  Node *left_child;

  if (!root || !*root) return;

  left_child = (*root)->left;
  (*root)->left = left_child->right;
  left_child->right = *root;
  *root = left_child;
}

int main(int argc, char *argv[]) {
  Node *tree;

  insertValue(&tree, 6);
  insertValue(&tree, 4);
  insertValue(&tree, 5);
  insertValue(&tree, 7);
  insertValue(&tree, 2);
  insertValue(&tree, 3);
  insertValue(&tree, 1);

  traverseTree(tree);

  printf("\nRotating\n\n");
  rotateRight(&tree);
  traverseTree(tree);

  return EXIT_SUCCESS;
}
