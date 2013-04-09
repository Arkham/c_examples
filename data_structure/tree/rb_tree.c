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

typedef struct rb_tree {
  node_t *root;
} rb_tree_t;

rb_tree_t *rb_create() {
  rb_tree_t *tree;

  if ( (tree = malloc(sizeof(rb_tree_t))) == NULL)
    return NULL;

  tree->root = NULL;
  return tree;
}

node_t *rb_create_node(int value) {
  node_t* new;

  if ( (new = malloc(sizeof(node_t))) == NULL)
    return NULL;

  new->color = RED;
  new->value = value;
  new->parent = new->left = new->right = NULL;

  return new;
}

node_t *rb_grandparent(node_t *current) {
  if (current != NULL && current->parent != NULL)
    return current->parent->parent;
  else
    return NULL;
}

node_t *rb_uncle(node_t *current) {
  node_t *grandpa;

  grandpa = rb_grandparent(current);
  if (grandpa == NULL) {
    return NULL;
  } else {
    if (current->parent == grandpa->left)
      return grandpa->right;
    else
      return grandpa->left;
  }
}

void rb_rotate_left(rb_tree_t *tree, node_t *node) {
  node_t *r_child = node->right;

  if (!r_child) return;

  node->right = r_child->left;
  if (r_child->left)
    r_child->left->parent = node;

  r_child->parent = node->parent;

  if (r_child->parent == NULL) {
    tree->root = r_child;
  } else {
    if (node == node->parent->left)
      node->parent->left = r_child;
    else
      node->parent->right = r_child;
  }

  r_child->left = node;
  node->parent = r_child;
}

void rb_rotate_right(rb_tree_t *tree, node_t *node) {
  node_t *l_child = node->left;

  if (!l_child) return;

  node->left = l_child->right;
  if (l_child->right) {
    l_child->right->parent = node;
  }

  if (node->parent == NULL)
    tree->root = l_child;
  else
    if (node == node->parent->left)
      node->parent->left = l_child;
    else
      node->parent->right = l_child;

  l_child->parent = node->parent;
  node->parent = l_child;
  l_child->right = node;
}

void rb_insert_coloring(rb_tree_t *tree, node_t *current) {
  node_t *u, *g;

  if (current->parent == NULL) {
    current->color = BLACK;
  } else if (current->parent->color == BLACK) {
    /* all good, rb tree properties fulfilled */
    return;
  } else {
    /* parent is red */
    u = rb_uncle(current);
    g = rb_grandparent(current);

    if (u != NULL && u->color == RED) {
      /* uncle is red */
      current->parent->color = BLACK;
      u->color = BLACK;
      g->color = RED;
      rb_insert_coloring(tree, g);
    } else {
      /* uncle is black or missing, rotation needed */

      /* child is on wrong side, rotate parent */
      if (current == current->parent->right && current->parent == g->left) {
        rb_rotate_left(tree, current->parent);
        current = current->left;
      } else if (current == current->parent->left && current->parent == g->right) {
        rb_rotate_right(tree, current->parent);
        current = current->right;
      }
      /* child on right side, rotate grandparent */
      current->parent->color = BLACK;
      g->color = RED;
      if (current == current->parent->left)
        rb_rotate_right(tree, g);
      else
        rb_rotate_left(tree, g);
    }
  }
}

node_t * rb_insert(rb_tree_t *tree, int value) {
  node_t *current = tree->root;
  node_t *new = rb_create_node(value);

  /* empty tree */
  if (!current) {
    tree->root = new;
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

  rb_insert_coloring(tree, new);
  return new;
}

void print_node(node_t *node) {
  if (node->color == BLACK)
    printf("B: ");
  else
    printf("R: ");
  printf("%d\n", node->value);
}

void rb_inorder_traverse(node_t *root) {
  if (!root) return;

  if (root->left)
    rb_inorder_traverse(root->left);

  print_node(root);

  if (root->right)
    rb_inorder_traverse(root->right);
}

void rb_preorder_traverse(node_t *root) {
  if (!root) return;

  print_node(root);

  if (root->left)
    rb_preorder_traverse(root->left);

  if (root->right)
    rb_preorder_traverse(root->right);
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

void rb_breadth_traverse(node_t *root) {
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

void rb_print_node_depth(node_t *node, int depth) {
  int i;

  for (i=0; i<depth; i++) {
    printf(" ");
  }
  printf("%3d\n", node->value);
}

void rb_depth_traverse(node_t *node, int depth) {
  if (!node) return;

  if (node->left)
    rb_depth_traverse(node->left, depth+3);

  rb_print_node_depth(node, depth);

  if (node->right)
    rb_depth_traverse(node->right, depth+3);
}

void rb_traverse(node_t *root) {
  rb_depth_traverse(root, 0);
}

int main(int argc, char *argv[]) {
  rb_tree_t *tree;

  tree = rb_create();

  rb_insert(tree, 1);
  rb_insert(tree, 2);
  rb_insert(tree, 3);
  rb_insert(tree, 4);
  rb_insert(tree, 5);
  rb_insert(tree, 6);
  rb_inorder_traverse(tree->root);
  printf("\n");
  rb_preorder_traverse(tree->root);
  printf("\n");
  rb_breadth_traverse(tree->root);
  printf("\n");
  rb_traverse(tree->root);

  return EXIT_SUCCESS;
}
