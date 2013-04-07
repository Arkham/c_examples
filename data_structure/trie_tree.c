#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_s {
  char key;
  int value;
  struct node_s *next;
  struct node_s *child;
};

typedef struct node_s node_t;

node_t *create_node(char key) {
  node_t *new;

  if ( (new = malloc(sizeof(node_t))) == NULL)
    return NULL;

  new->key = key;
  new->value = -1;
  new->next = NULL;
  new->child = NULL;

  return new;
}

node_t *tt_create() {
  return create_node(0);
}

void tt_set(node_t *root, char *key, int value) {
  int i, length;
  node_t *current, *child, *last, *new;

  if (!root) return;
  current = root;
  length = strlen(key);

  for (i=0; i<length; i++) {
    child = current->child;

    while (child != NULL && child->key != key[i]) {
      last = child;
      child = child->next;
    }

    if (child != NULL) {
      /* child found */
      if (i == length-1) {
        child->value = value;
      } else {
        current = child;
      }
    } else {
      /* have to alloc child */
      new = create_node(key[i]);

      if (current->child == NULL) {
        current->child = new;
      } else {
        last->next = new;
      }

      if (i == length-1) {
        new->value = value;
      } else {
        current = new;
      }
    }
  }
}

int tt_get(node_t *root, char *key) {
  int i, length;
  node_t *current, *child;

  if (!root) return -1;
  length = strlen(key);
  current = root;

  for (i=0; i<length; i++) {
    child = current->child;

    while (child != NULL && child->key != key[i])
      child = child->next;

    if (child != NULL) {
      /* child found */
      if (i == length-1) {
        return child->value;
      } else {
        current = child;
      }
    } else {
      /* not found */
      break;
    }
  }

  return -1;
}

void tt_print(node_t *root, char *key) {
  printf("%s: %d\n", key, tt_get(root, key));
}

void depth_traverse(node_t *root, int level) {
  if (!root) return;

  printf("level %d: %c\n", level, root->key);

  if (root->child)
    depth_traverse(root->child, level+1);

  if (root->next)
    depth_traverse(root->next, level);
}

typedef struct queue_t {
  struct queue_t *next;
  void *value;
  int level;
} queue_t;

void push(queue_t **queue, void *value, int level) {
  queue_t *current, *new;

  if (!queue) return;

  new = malloc(sizeof(queue_t));
  if (!new) return;
  new->value = value;
  new->level = level;
  new->next = NULL;

  current = *queue;

  if (!current) {
    *queue = new;
  } else {
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new;
  }
}

int pop(queue_t **queue, void **result) {
  queue_t *current;

  if (!queue || !*queue) return -1;

  current = *queue;

  if (current->next == NULL) {
    *queue = NULL;
  } else {
    *queue = current->next;
  }

  *result = current->value;
  return current->level;
}

void breadth_traverse(node_t *root) {
  int level=0;
  node_t *popped, *child;
  queue_t *queue = NULL;

  if (!root) return;

  push(&queue, root, 0);

  while (queue != NULL) {
    level = pop(&queue, (void **) &popped);
    if (!popped) break;

    printf("level %d: %c\n", level, popped->key);

    child = popped->child;
    while (child != NULL) {
      push(&queue, child, level+1);
      child = child->next;
    }
  }
}

int main(int argc, char *argv[]) {
  node_t *tree = NULL;

  tree = tt_create();

  tt_set(tree, "ciro", 10);
  tt_set(tree, "ciao", 15);
  tt_set(tree, "cielo", 20);

  tt_print(tree, "ciro");
  tt_print(tree, "ciao");
  tt_print(tree, "cielo");

  depth_traverse(tree, 0);
  breadth_traverse(tree);

  return EXIT_SUCCESS;
}
