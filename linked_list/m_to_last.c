#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
  struct node *next;
  int data;
} node_t;

/* lib {{{ */
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

/* }}} */

/*
// iterate over the list once to find n
// iterate again to find n - m element
// requires two runs of the list
// O(n)

// iterate over the list once keeping all values
// just find the (n-m)th element once we get to the end of the list
// requires only one one of the list, altough we have to keep all values in memory
// O(n) space inefficient

// we know m
// so if we suppose that each element we get is the last
// we just need to remember the last minus m (if it exists)
// iterate over the list only once O(n)
*/

bool m_to_last(node_t *head, int m_counter, node_t **found) {
  node_t *current = head;
  bool synced = false;

  *found = NULL;

  if (!head || m_counter < 0)
    return false;

  while (current != NULL) {
    if (m_counter > 0) {
      m_counter--;
    } else if (m_counter == 0) {
      if (synced) {
        *found = (*found)->next;
      } else {
        *found = head;
        synced = true;
      }
    }

    current = current->next;
  }

  return synced;
}

int main(int argc, char *argv[]) {
  int i;
  node_t *head;
  node_t *found;

  for (i=0; i<10; i++)
    insertTail(&head, i);
  traverseList(head);

  for (i=0; i<13; i++) {
    if (m_to_last(head, i, &found))
      printf("Found %d at %d from last\n", found->data, i);
    else
      printf("Could not find anything at %d from last\n", i);
  }

  return EXIT_SUCCESS;
}
