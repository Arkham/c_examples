#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element {
  struct Element *next;
  int data;
} Element;

Element * createElement(int value) {
  Element *new;

  new = malloc(sizeof(Element));
  if (!new) return NULL;

  new->data = value;
  new->next = NULL;
  return new;
}

/* void removeHead(Element *head) { */
/*   free(head); */
/*   head = head->next; */
/* } */

void removeHead(Element **head) {
  Element *temp = *head;

  if (!head || !*head)
    return;

  *head = (*head)->next;
  free(temp);
}

void printHead(Element *head) {
  if (head)
    printf("List head is %d\n", head->data);
  else
    printf("List is empty\n");
}

int main(int argc, char *argv[]) {
  Element *head;

  printHead(head);

  head = createElement(10);
  if (!head) return EXIT_FAILURE;
  printHead(head);

  removeHead(&head);
  printHead(head);

  return EXIT_SUCCESS;
}
