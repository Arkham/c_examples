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

void traverseNext(Element *head, int indent, bool include_child) {
  Element *current = head;

  if (!head) return;

  if (!indent)
    printf("\nFrom head:\n");

  while (current) {
    printIndent(indent);
    printf("Value is %d\n", current->value);

    if (include_child && current->child)
      traverseNext(current->child, indent+2, include_child);

    current = current->next;
  }
}

void traverseDeep(Element *head) {
  traverseNext(head, 0, true);
}

void traverseSimple(Element *head) {
  traverseNext(head, 0, false);
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

/* flattening {{{ */
void appendList(List **list, Element *head) {
  Element *current = head, *tail;

  if (!list || !*list || !head) return;
  tail = (*list)->tail;

  while (current) {
    current->prev = tail;
    tail->next = current;
    tail = current;

    current = current->next;
  }

  (*list)->tail = tail;
}

void flattenList(List **list) {
  Element *current;

  if (!list || !*list) return;

  current = (*list)->head;
  while (current) {
    if (current->child)
      appendList(list, current->child);
    current = current->next;
  }
}
/* }}} */

/* unflattening {{{ */
void breakList(Element *head) {
  Element *current = head;

  if (!head) return;

  while (current) {
    if (current->child) {
      /* break off list */
      current->child->prev->next = NULL;
      breakList(current->child);
    }
    current = current->next;
  }
}

void unflattenList(List **list) {
  Element *current;

  if (!list || !*list) return;
  current = (*list)->head;

  breakList(current);

  while (current->next) { current = current->next; }
  (*list)->tail = current;
}
/* }}} */

int main(int argc, char *argv[]) {
  List *list;
  int i;

  list = createList();
  if (!list) return EXIT_FAILURE;

  for (i = 0; i < 3; ++i)
    insertElement(&list, i);
  traverseDeep(list->head);

  for (i = 5; i < 8; ++i)
    insertAfter(&(list->head->child), i);
  traverseDeep(list->head);

  for (i = 10; i < 14; ++i)
    insertAfter(&(list->tail->child), i);
  traverseDeep(list->head);

  flattenList(&list);
  traverseDeep(list->head);

  unflattenList(&list);
  traverseDeep(list->head);

  return EXIT_SUCCESS;
}
