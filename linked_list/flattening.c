#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* lib {{{ */
typedef struct Element {
  struct Element *next;
  struct Element *prev;
  struct Element *child;
  int value;
} Element;

typedef struct List {
  Element *head;
  Element *tail;
} List;

Element * createElement(int value) {
  Element *new;

  new = malloc(sizeof(Element));
  if (!new) return NULL;

  new->value = value;
  new->next = NULL;
  new->prev = NULL;
  new->child = NULL;

  return new;
}

void insertElement(List **list, int value) {
  Element *new;
  Element *current;

  if (!list || !*list) return;
  if (!(new = createElement(value))) return;

  if (!(*list)->head) {
    (*list)->head = new;
    (*list)->tail = new;
    return;
  }

  current = (*list)->head;
  while (current->next)
    current = current->next;

  current->next = new;
  new->prev = current;

  (*list)->tail = new;

  return;
}

void printIndent(int indent) {
  int i;

  for (i=0; i<indent; i++)
    printf(" ");
}

void traverseNext(Element *head, int indent) {
  Element *current = head;

  if (!head) return;

  if (!indent)
    printf("\nFrom head:\n");

  while (current) {
    printIndent(indent);
    printf("Value is %d\n", current->value);

    if (current->child)
      traverseNext(current->child, indent+2);

    current = current->next;
  }
}

bool insertAfter(Element **head, int value) {
  Element *current, *new;

  if (!head) return false;

  new = createElement(value);
  if (!new) return false;

  current = *head;
  if (current == NULL) {
    *head = new;
    return true;
  }

  while (current->next)
    current = current->next;

  current->next = new;
  new->prev = current;

  return true;
}

List * createList() {
  List *new;

  new = malloc(sizeof(List));
  if (!new) return NULL;

  new->head = NULL;
  new->tail = NULL;

  return new;
}

/* }}} */

void appendList(List **list, Element **head) {
  Element *current, *tail;

  if (!list || !*list || !head || !*head) return;

  current = *head;
  tail = (*list)->tail;

  while (current) {
    if (tail) {
      (*list)->tail->next = current;
    }

    (*list)->tail = current;
    current = current->next;
  }

  *head = NULL;
}

void flattenList(List **list) {
  Element *current;

  if (!list || !*list) return;

  current = (*list)->head;
  while (current) {
    if (current->child)
      appendList(list, &(current->child));
    current = current->next;
  }
}

int main(int argc, char *argv[]) {
  List *list;
  int i;

  list = createList();
  if (!list) return EXIT_FAILURE;

  for (i = 0; i < 3; ++i)
    insertElement(&list, i);
  traverseNext(list->head, 0);

  for (i = 5; i < 8; ++i)
    insertAfter(&(list->head->child), i);
  traverseNext(list->head, 0);

  for (i = 10; i < 14; ++i)
    insertAfter(&(list->tail->child), i);
  traverseNext(list->head, 0);

  flattenList(&list);
  traverseNext(list->head, 0);

  return EXIT_SUCCESS;
}
