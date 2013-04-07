#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum color { RED, BLACK } color_t;

typedef struct node {
  int value;
  color_t color;
  struct node *left;
  struct node *right;
  struct node *parent;
} node_t;

node_t *create_node(int value) {
  node_t* new;

  if ( (new = malloc(sizeof(node_t))) == NULL)
    return NULL;

  new->color = RED;
  new->value = value;
  new->parent = new->left = new->right = NULL;

  return new;
}

node_t *grandparent(node_t *current) {
  if (current != NULL && current->parent != NULL)
    return current->parent->parent;
  else
    return NULL;
}

node_t *uncle(node_t *current) {
  node_t *grandpa;

  grandpa = grandparent(current);
  if (grandpa == NULL) {
    return NULL;
  } else {
    if (current->parent == grandpa->left)
      return grandpa->right;
    else
      return grandpa->left;
  }
}

void rotate_left(node_t **root, node_t *node) {
  node_t *r_child = node->right;

  if (!r_child) return;

  node->right = r_child->left;
  if (r_child->left)
    r_child->left->parent = node;

  r_child->parent = node->parent;

  if (r_child->parent == NULL) {
    *root = r_child;
  } else {
    if (node == node->parent->left)
      node->parent->left = r_child;
    else
      node->parent->right = r_child;
  }

  r_child->left = node;
  node->parent = r_child;
}

void rotate_right(node_t **root, node_t *node) {
  node_t *l_child;

  if (!root || !node) return;

  l_child = node->left;
  if (!l_child) return;

  node->left = l_child->right;
  if (l_child->right) {
    l_child->right->parent = node;
  }

  if (node->parent == NULL)
    *root = l_child;
  else
    if (node == node->parent->left)
      node->parent->left = l_child;
    else
      node->parent->right = l_child;

  l_child->parent = node->parent;
  node->parent = l_child;
  l_child->right = node;
}

void rb_insert_coloring(node_t **root, node_t *current) {
  node_t *u, *g;

  if (current->parent == NULL) {
    current->color = BLACK;
  } else if (current->parent->color == BLACK) {
    /* all good, rb tree properties fulfilled */
    return;
  } else {
    /* parent is red */
    u = uncle(current);
    g = grandparent(current);

    if (u != NULL && u->color == RED) {
      /* uncle is red */
      current->parent->color = BLACK;
      u->color = BLACK;
      g->color = RED;
      rb_insert_coloring(root, g);
    } else {
      /* uncle is black or missing, rotation needed */

      /* child is on wrong side, rotate parent */
      if (current == current->parent->right && current->parent == g->left) {
        rotate_left(root, current->parent);
        current = current->left;
      } else if (current == current->parent->left && current->parent == g->right) {
        rotate_right(root, current->parent);
        current = current->right;
      }
      /* child on right side, rotate grandparent */
      current->parent->color = BLACK;
      g->color = RED;
      if (current == current->parent->left)
        rotate_right(root, g);
      else
        rotate_left(root, g);
    }
  }
}

node_t * rb_insert(node_t **root, int value) {
  node_t *current, *new;

  if (!root) return NULL;
  current = *root;
  new = create_node(value);

  /* empty tree */
  if (!current) {
    *root = new;
  }

  while (current != NULL) {
    if (value > current->value) {
      if (current->right) {
        current = current->right;
      } else {
        current->right = new;
        new->parent = current;
        break;
      }
    } else {
      if (current->left)
        current = current->left;
      else {
        current->left = new;
        new->parent = current;
        break;
      }
    }
  }

  rb_insert_coloring(root, new);
  return new;
}

void print_node(node_t *node) {
  if (node->color == BLACK)
    printf("B: ");
  else
    printf("R: ");
  printf("%d\n", node->value);
}

void rb_traverse(node_t *root) {
  if (!root) return;

  if (root->left)
    rb_traverse(root->left);

  print_node(root);

  if (root->right)
    rb_traverse(root->right);
}

void rb_inorder_traverse(node_t *root) {
  if (!root) return;

  print_node(root);

  if (root->left)
    rb_traverse(root->left);

  if (root->right)
    rb_traverse(root->right);
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
  node_t *popped;
  queue_t *queue = NULL;

  if (!root) return;

  push(&queue, root, 0);

  while (queue != NULL) {
    level = pop(&queue, (void **) &popped);
    if (!popped) break;

    printf("level %d: %d\n", level, popped->value);

    if (popped->left) {
      push(&queue, popped->left, level+1);
    }

    if (popped->right) {
      push(&queue, popped->right, level+1);
    }
  }
}


int main(int argc, char *argv[]) {
  node_t *rb_tree;

  rb_insert(&rb_tree, 1);
  rb_insert(&rb_tree, 2);
  rb_insert(&rb_tree, 3);
  rb_insert(&rb_tree, 4);
  rb_insert(&rb_tree, 5);
  rb_insert(&rb_tree, 6);
  rb_traverse(rb_tree);
  printf("\n");
  rb_inorder_traverse(rb_tree);
  printf("\n");
  breadth_traverse(rb_tree);

  return EXIT_SUCCESS;
}
