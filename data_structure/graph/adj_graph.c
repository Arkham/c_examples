#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct adj_list {
  int count;
  int length;
  bool is_sorted;
  int list[1];
} adj_list_t;

typedef struct adj_graph {
  int vertices;
  int edges;
  adj_list_t * adj_list[1];
} adj_graph_t;

adj_graph_t *adj_create(int size) {
  adj_graph_t *graph;
  int i;

  graph = malloc(sizeof(adj_graph_t) + sizeof(adj_list_t *) * (size - 1));
  if (!graph) return NULL;

  graph->vertices = size;
  graph->edges = 0;

  for (i=0; i<size; i++) {
    graph->adj_list[i] = malloc(sizeof(adj_list_t));
    if (!graph->adj_list[i]) return NULL;

    graph->adj_list[i]->count = 0;
    graph->adj_list[i]->length = 1;
    graph->adj_list[i]->is_sorted = true;
  }

  return graph;
}

void adj_destroy(adj_graph_t *graph) {
  int i;

  if (!graph) return;
  for (i=0; i<graph->vertices; i++) {
    free(graph->adj_list[i]);
  }
  free(graph);
}

void adj_add_edge(adj_graph_t *graph, int source, int dest) {
  adj_list_t *adj_list;

  if (!graph || source < 0 || source >= graph->vertices || dest < 0 || dest >= graph->vertices)
    return;

  adj_list = graph->adj_list[source];
  while (adj_list->count >= adj_list->length) {
    adj_list->length *= 2;
    adj_list = graph->adj_list[source] =
      realloc(graph->adj_list[source],
          sizeof(adj_list_t) + sizeof(int) * (adj_list->length - 1));
  }

  adj_list->list[adj_list->count++] = dest;
  adj_list->is_sorted = false;

  graph->edges++;
}

int adj_vertex_count(adj_graph_t *graph) {
  return graph ? graph->vertices : -1;
}

int adj_edge_count(adj_graph_t *graph) {
  return graph ? graph->edges : -1;
}

int adj_out_degree(adj_graph_t *graph, int source) {
  if (!graph || source < 0 || source >= graph->vertices || !graph->adj_list[source] )
    return -1;

  return graph->adj_list[source]->count;
}

#define BSEARCH_THRESHOLD 1

static int intcmp(const void *first, const void *second) {
  return *((const int *) first) - *((const int *) second);
}

bool adj_edge_exist(adj_graph_t *graph, int source, int dest) {
  int i;
  adj_list_t *adj_list;

  if (!graph || source < 0 || source >= graph->vertices || dest < 0 || dest >= graph->vertices)
    return -1;

  adj_list = graph->adj_list[source];

  if (adj_out_degree(graph, source) >= BSEARCH_THRESHOLD) {
    if (! adj_list->is_sorted) {
      qsort(adj_list->list, adj_list->count, sizeof(int), intcmp);
    }

    return bsearch(&dest,
        adj_list->list, adj_list->count, sizeof(int), intcmp) != 0;

  } else {
    for (i=0; i<adj_list->count; i++)
      if (adj_list->list[i] == dest) return true;
    /* else */
    return false;
  }
}

void adj_foreach(adj_graph_t *graph, int source,
    void (*f) (adj_graph_t *graph, int source, int dest, void *data),
    void *data) 
{
  int i;
  adj_list_t * adj_list;

  if (!graph || source < 0 || source >= graph->vertices || ! graph->adj_list[source] )
    return;
  adj_list = graph->adj_list[source];

  for (i=0; i<adj_list->count; i++) {
    f(graph, source, adj_list->list[i], data);
  }
}

void adj_print_edge(adj_graph_t *graph, int source, int dest, void *data) {
  printf("%d -> %d\n", source, dest);
}

void adj_print_graph(adj_graph_t *graph) {
  int i;

  if (!graph) return;
  for (i=0; i<graph->vertices; i++)
    adj_foreach(graph, i, &adj_print_edge, NULL);
}

int main(int argc, char *argv[]) {
  adj_graph_t *graph;

  graph = adj_create(16);
  adj_add_edge(graph, 0, 5);
  adj_add_edge(graph, 0, 2);
  adj_add_edge(graph, 0, 3);
  adj_add_edge(graph, 2, 4);
  adj_add_edge(graph, 5, 0);
  adj_print_graph(graph);

  return EXIT_SUCCESS;
}
