#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

typedef struct avl_node {
  struct avl_node *left;
  struct avl_node *right;
  int value;
} avl_node_t;

typedef struct avl_tree {
  struct avl_node *root;
} avl_tree_t;

avl_tree_t *avl_create() {
  avl_tree_t *tree = NULL;

  if ( (tree = malloc(sizeof(avl_tree_t))) == NULL)
    return NULL;

  tree->root = NULL;
  return tree;
}

avl_node_t *avl_create_node(int value) {
  avl_node_t *node;

  if ( (node = malloc(sizeof(avl_node_t))) == NULL)
    return NULL;

  node->left = NULL;
  node->right = NULL;
  node->value = value;

  return node;
}

int avl_node_height(avl_node_t *node) {
  int left=0, right=0;

  if (!node) return 0;

  if (node->left)
    left = avl_node_height(node->left);

  if (node->right)
    right = avl_node_height(node->right);

  return left > right ? ++left : ++right;
}

int avl_balance_factor(avl_node_t *node) {
  int bf = 0;

  if (node->left)
    bf += avl_node_height(node->left);

  if (node->right)
    bf -= avl_node_height(node->right);

  return bf;
}

avl_node_t *avl_rotate_leftleft(avl_node_t *node) {
  avl_node_t *a = node;
  avl_node_t *b = a->left;

  a->left = b->right;
  b->right = a;

  return b;
}

avl_node_t *avl_rotate_rightright(avl_node_t *node) {
  avl_node_t *a = node;
  avl_node_t *b = a->right;

  a->right = b->left;
  b->left = a;

  return b;
}

avl_node_t *avl_rotate_leftright(avl_node_t *node) {
  avl_node_t *a = node;
  avl_node_t *b = a->left;

  a->left = avl_rotate_rightright(b);
  return avl_rotate_leftleft(a);
}

avl_node_t *avl_rotate_rightleft(avl_node_t *node) {
  avl_node_t *a = node;
  avl_node_t *b = a->right;

  a->right = avl_rotate_leftleft(b);
  return avl_rotate_rightright(a);
}

avl_node_t * avl_balance_node(avl_node_t *node) {
  int bf;

  if (!node) return NULL;

  if (node->left)
    node->left = avl_balance_node(node->left);
  if (node->right)
    node->right = avl_balance_node(node->right);

  bf = avl_balance_factor(node);

  if (bf > 1) {
    if (avl_balance_factor(node->left) < 0)
      node = avl_rotate_leftright(node);
    else
      node = avl_rotate_leftleft(node);
  } else if (bf < -1) {
    if (avl_balance_factor(node->right) > 0)
      node = avl_rotate_rightleft(node);
    else
      node = avl_rotate_rightright(node);
  }

  return node;
}

void avl_balance(avl_tree_t *tree) {
  avl_node_t *newroot = NULL;

  newroot = avl_balance_node(tree->root);

  if (newroot != tree->root) {
    tree->root = newroot;
  }
}

void avl_insert(avl_tree_t *tree, int value) {
  avl_node_t *current = NULL;
  avl_node_t *new = NULL;

  if (tree) {
    current = tree->root;
    new = avl_create_node(value);
  }

  if (!current) {
    tree->root = new;
    return;
  }

  while (current != NULL) {
    if (value < current->value) {
      if (current->left) {
        current = current->left;
      } else {
        current->left = new;
        break;
      }
    } else if (value > current->value) {
      if (current->right) {
        current = current->right;
      } else {
        current->right = new;
        break;
      }
    } else {
      /* oops, we shouldn't be here */
      break;
    }
  }

  avl_balance(tree);
}

void avl_print_node_depth(avl_node_t* node, int depth) {
  int i;

  for (i=0; i<depth; i++)
    printf(" ");

  printf("%2d \n", node->value);
}

void avl_depth_traverse(avl_node_t* node, int depth) {
  if (!node) return;

  if (node->left)
    avl_depth_traverse(node->left, depth+3);

  avl_print_node_depth(node, depth);

  if (node->right)
    avl_depth_traverse(node->right, depth+3);
}

void avl_traverse(avl_tree_t* tree) {
  avl_depth_traverse(tree->root, 0);
  printf("\n");
}

int main(int argc, char *argv[]) {
  avl_tree_t *tree;

  tree = avl_create();
  avl_insert(tree, 10);
  avl_insert(tree, 5);
  avl_insert(tree, 2);
  avl_insert(tree, 20);
  avl_insert(tree, 7);
  avl_insert(tree, 3);
  avl_insert(tree, 12);
  avl_insert(tree, 4);
  avl_insert(tree, 6);
  avl_insert(tree, 21);
  avl_insert(tree, 22);
  avl_insert(tree, 23);
  avl_traverse(tree);

  return EXIT_SUCCESS;
}
