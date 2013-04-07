#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element {
  struct Element *next;
  int data;
} Element;

Element *head = NULL;
Element *tail = NULL;

Element *createElement(int data) {
  Element *new;

  new = (Element *) malloc(sizeof(Element));
  if (!new) return NULL;

  new->data = data;
  new->next = NULL;
  return new;
}

bool delete(Element *elem) {
  Element *current;
  Element *previous;

  if (!head || !tail || !elem) return false;

  if (elem == head && elem == tail) {
    head = NULL;
    tail = NULL;
    free(elem);
    return true;
  }

  current = head;

  while (current != NULL && current != elem) {
    previous = current;
    current = current->next;
  }

  if (current == NULL) return false;

  if (current == head) {
    head = head->next;
  } else if (current == tail) {
    tail = previous;
    previous->next = NULL;
  } else {
    previous->next = current->next;
  }

  free(current);

  return true;
}

bool insertAfter(Element *elem, int data) {
  Element *new;
  Element *current;
  Element *previous;

  new = createElement(data);
  if (!new) return false;

  if (elem == NULL) {
    new->next = head;
    head = new;

    if (tail == NULL) {
      tail = new;
    }

    return true;
  }

  current = head;

  while (current != NULL && current != elem) {
    previous = current;
    current = current->next;
  }

  if (current == NULL) {
    previous->next = new;
    tail = new;
  } else {
    if (current == tail)
      tail = new;

    new->next = current->next;
    current->next = new;
  }

  return true;
}

void traverse(Element *head) {
  Element *temp = head;

  while (temp != NULL) {
    printf("%d\n", temp->data);
    temp = temp->next;
  }
}

void checkEdges() {
  if (head)
    printf("Head is %d\n", head->data);
  else
    printf("Head is NULL\n");

  if (tail)
    printf("Tail is %d\n", tail->data);
  else
    printf("Tail is NULL\n");

  printf("\n");
}

int main(int argc, char *argv[]) {

  Element *fake, *deleteme;

  fake = malloc(sizeof(Element));

  checkEdges();

  insertAfter(NULL, -1);
  checkEdges();

  insertAfter(head, 1);
  checkEdges();

  insertAfter(fake, 2);
  checkEdges();

  deleteme = tail;

  insertAfter(tail, 3);
  checkEdges();

  insertAfter(head, 0);
  traverse(head);
  checkEdges();

  delete(deleteme);
  traverse(head);
  checkEdges();

  delete(head);
  traverse(head);
  checkEdges();

  delete(tail);
  traverse(head);
  checkEdges();

  delete(tail);
  traverse(head);
  checkEdges();

  delete(head);
  traverse(head);
  checkEdges();

  delete(head);
  traverse(head);
  checkEdges();

  free(fake);

  return EXIT_SUCCESS;
}
