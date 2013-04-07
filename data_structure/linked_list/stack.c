#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element {
  struct Element *next;
  int data;
} Element;

Element * createStack() {
  return NULL;
}

void destroyStack(Element **stack) {
  Element *current, *delete;

  current = *stack;
  delete = current;

  while (current != NULL) {
    current = current->next;
    free(delete);
    delete = current;
  }

  *stack = NULL;
}

Element *createElement(int value) {
  Element *new;

  new = (Element *) malloc(sizeof(Element));
  if (!new) return NULL;

  new->data = value;
  new->next = NULL;
  return new;
}

bool push(Element **stack, int value) {
  Element *current = *stack;
  Element *new;

  new = createElement(value);
  if (!new) return false;

  if (current == NULL) {
    *stack = new;
    return true;
  }

  new->next = *stack;
  *stack = new;
  return true;
}

bool pop(Element **stack, int *result) {
  Element *current = *stack;

  if (current == NULL)
    return false;

  *result = current->data;
  *stack = current->next;
  free(current);
  return true;
}

int main(int argc, char *argv[]) {
  Element *stack;
  int result;

  stack = createStack();

  if (push(&stack, 0)) printf("Pushed 0\n");
  else printf("Could not push!\n");
  if (push(&stack, 1)) printf("Pushed 1\n");
  else printf("Could not push!\n");

  if (pop(&stack, &result)) printf("Popped %d\n", result);
  else printf("Could not pop!\n");

  if (pop(&stack, &result)) printf("Popped %d\n", result);
  else printf("Could not pop!\n");

  if (pop(&stack, &result)) printf("Popped %d\n", result);
  else printf("Could not pop!\n");

  push(&stack, 2);

  destroyStack(&stack);

  return EXIT_SUCCESS;
}
