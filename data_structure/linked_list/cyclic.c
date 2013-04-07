#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element {
  struct Element *next;
  int value;
} Element;

Element *createElement(int value) {
  Element *new;

  new = malloc(sizeof(Element));
  if (!new) return NULL;

  new->next = NULL;
  new->value = value;
  return new;
}

bool insert(Element **head, int value) {
  Element *current = *head, *new;

  if (!(new = createElement(value))) return false;

  if (current) {
    while (current->next) { current = current->next; }
    current->next = new;
  } else {
    *head = new;
  }

  return true;
}

void traverse(Element *head) {
  while (head) {
    printf("%d\n", head->value);
    head = head->next;
  }
  printf("EOL\n");
}

void makeCyclic(Element *head) {
  Element *original = head;

  if (!head) return;

  while (head->next) { head = head->next; }
  head->next = original;
}

bool isCyclic(Element *head) {
  Element *fast, *slow;

  slow = head;
  fast = head->next;

  while (1) {
    if (!fast || !fast->next) return false;
    if (slow == fast || slow == fast->next) return true;

    slow = slow->next;
    fast = fast->next->next;
  }
}

int main(int argc, char *argv[]) {
  Element *acyclic;
  Element *cyclic;
  int i;

  for (i = 0; i < 4; i++) {
    insert(&acyclic, i);
  }
  traverse(acyclic);

  for (i = 0; i < 4; i++) {
    insert(&cyclic, i);
  }
  makeCyclic(cyclic);
  /* THIS LOOPS FOREVER */
  /* traverse(cyclic); */

  printf("acyclic is cyclic: %d\n", isCyclic(acyclic));
  printf("cyclic is cyclic: %d\n", isCyclic(cyclic));

  return EXIT_SUCCESS;
}
