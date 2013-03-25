#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct Node {
  struct Node *left;
  struct Node *right;
  struct Node *next;
  int value;
} Node;

Node *createNode(value) {
  Node *new;

  new = malloc(sizeof(Node));
  if (!new) return NULL;

  new->left = NULL;
  new->right = NULL;
  new->next = NULL;
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

void printNode(Node *node) {
  printf("Value: %d\n", node->value);
}

void inorder(Node *root) {
  if (!root) return;

  inorder(root->left);
  printNode(root);
  inorder(root->right);
}

void preorder(Node *root) {
  if (!root) return;

  printNode(root);
  preorder(root->left);
  preorder(root->right);
}

void postorder(Node *root) {
  if (!root) return;

  postorder(root->left);
  postorder(root->right);
  printNode(root);
}

int height(Node *root) {
  if (!root) return 0;

  return 1 + MAX(height(root->left), height(root->right));
}

bool push(Node **stack, Node *elem) {
  Node *current;

  if (!stack) return false;
  current = *stack;

  if (!current) {
    *stack = elem;
    return true;
  }

  while (current->next) { current = current->next; }
  current->next = elem;
  return true;
}

bool pop(Node **stack, Node **elem) {
  Node *current;

  if (!stack || !*stack) return false;
  current = *stack;

  if (!current->next) {
    *elem = current;
    *stack = NULL;
    return true;
  }

  while (current->next->next) { current = current->next; }
  *elem = current->next;
  current->next = NULL;
  return true;
}

void traverseStack(Node *stack) {
  while (stack) {
    printf("Stack value: %d\n", stack->value);
    stack = stack->next;
  }
}

void imperative_preorder(Node *root) {
  Node *stack = NULL, *current;

  push(&stack, root);

  while (stack) {
    pop(&stack, &current);
    printNode(current);

    if (current->right) push(&stack, current->right);
    if (current->left) push(&stack, current->left);
  }
}

int main(int argc, char *argv[]) {
  Node *root;

  insert(&root, 5);
  insert(&root, 1);
  insert(&root, 10);
  insert(&root, 6);
  insert(&root, 12);
  insert(&root, 14);

  preorder(root);
  printf("\n");
  inorder(root);
  printf("\n");
  postorder(root);
  printf("\n");

  printf("Tree height: %d\n", height(root));

  imperative_preorder(root);

  return EXIT_SUCCESS;
}
