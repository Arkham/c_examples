#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
  struct node *next;
  int data;
} node_t;

node_t * createNode(int value) {
  node_t *current;

  current = (node_t *) malloc(sizeof(node_t));
  if (!current) return NULL;

  current->data = value;
  current->next = NULL;

  return current;
}

bool insertHead(node_t **head, int value) {
  node_t *current;

  current = createNode(value);
  if (!current) return false;

  current->next = *head;
  *head = current;

  return true;
}

bool insertTail(node_t **head, int value) {
  node_t *temp = *head;
  node_t *current;

  if (temp == NULL)
    return insertHead(head, value);

  while (temp->next != NULL) {
    temp = temp->next;
  }

  current = createNode(value);
  if (!current) return false;
  temp->next = current;

  return true;
}

void traverseList(node_t *head) {
  while (head != NULL) {
    printf("%d\n", head->data);
    head = head->next;
  }
  printf("EOL\n");
}

bool removeNode(node_t **head, int value) {
  node_t *temp;
  node_t *delete;

  if (!head || !*head)
    return false;

  temp = *head;

  if (temp->data == value) {
    *head = temp->next;
    free(temp);
    return true;
  }

  delete = temp->next;

  while (delete != NULL && delete->data != value) {
    temp = delete;
    delete = delete->next;
  }

  if (delete == NULL) {
    return false;
  } else {
    temp->next = delete->next;
    free(delete);
    return true;
  }
}

bool emptyList(node_t **head) {
  node_t *delete = *head;
  node_t *current = delete;

  while(current != NULL) {
    current = current->next;
    free(delete);
    delete = current;
  }

  *head = NULL;

  return true;
}

int main(int argc, char *argv[]) {
  node_t *head = NULL;

  removeNode(&head, 666);
  emptyList(&head);

  insertTail(&head, 4);
  insertHead(&head, 3);
  insertHead(&head, 2);
  insertHead(&head, 1);
  insertHead(&head, 0);
  insertTail(&head, 5);
  insertTail(&head, 6);

  traverseList(head);

  if (removeNode(&head, 0)) printf("Removed 0\n");
  else printf("Failed to remove 0\n");

  if (removeNode(&head, 1)) printf("Removed 1\n");
  else printf("Failed to remove 1\n");

  if (removeNode(&head, 5)) printf("Removed 5\n");
  else printf("Failed to remove 5\n");

  if (removeNode(&head, 9)) printf("Removed 9\n");
  else printf("Failed to remove 9\n");

  traverseList(head);

  printf("Emptying list..\n");
  emptyList(&head);

  traverseList(head);

  return EXIT_SUCCESS;
}
