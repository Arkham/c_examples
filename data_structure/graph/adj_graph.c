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

void adj_add_edge(adj_graph_t *graph, int source, int sink) {
  adj_list_t *adj_list;

  if (!graph || source < 0 || source >= graph->vertices || sink < 0 || sink >= graph->vertices)
    return;

  adj_list = graph->adj_list[source];
  while (adj_list->count >= adj_list->length) {
    adj_list->length *= 2;
    adj_list = graph->adj_list[source] =
      realloc(graph->adj_list[source],
          sizeof(adj_list_t) + sizeof(int) * (adj_list->length - 1));
  }

  adj_list->list[adj_list->count++] = sink;
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

bool adj_edge_exist(adj_graph_t *graph, int source, int sink) {
  int i;
  adj_list_t *adj_list;

  if (!graph || source < 0 || source >= graph->vertices || sink < 0 || sink >= graph->vertices)
    return -1;

  adj_list = graph->adj_list[source];

  if (adj_out_degree(graph, source) >= BSEARCH_THRESHOLD) {
    if (! adj_list->is_sorted) {
      qsort(adj_list->list, adj_list->count, sizeof(int), intcmp);
    }

    return bsearch(&sink,
        adj_list->list, adj_list->count, sizeof(int), intcmp) != 0;

  } else {
    for (i=0; i<adj_list->count; i++)
      if (adj_list->list[i] == sink) return true;
    /* else */
    return false;
  }
}

void adj_foreach(adj_graph_t *graph, int source,
    void (*f) (adj_graph_t *graph, int source, int sink, void *data),
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

void adj_print_edge(adj_graph_t *graph, int source, int sink, void *data) {
  printf("%d -> %d\n", source, sink);
}

void adj_print_graph(adj_graph_t *graph) {
  int i;

  if (!graph) return;
  for (i=0; i<graph->vertices; i++)
    adj_foreach(graph, i, &adj_print_edge, NULL);
}

#define SEARCH_INFO_NULL (-1)

typedef struct search_info {
  adj_graph_t *graph;
  int reached;
  int *preorder;
  int *time;
  int *parent;
  int *depth;
} search_info_t;

/* void depth_first_search(search_info_t *info, int root); */
/* void breadth_first_search(search_info_t *info, int root); */

static int * create_empty_array(int size) {
  int *new, i;

  new = malloc(sizeof(int) * size);
  if (!new) return NULL;

  for (i=0; i<size; i++) {
    new[i] = SEARCH_INFO_NULL;
  }

  return new;
}

search_info_t * info_create(adj_graph_t *graph) {
  search_info_t *info;
  int n;

  info = malloc(sizeof(search_info_t));
  if (!info) return NULL;

  info->graph = graph;
  info->reached = 0;

  n = adj_vertex_count(graph);

  info->preorder = create_empty_array(n);
  info->time = create_empty_array(n);
  info->parent = create_empty_array(n);
  info->depth = create_empty_array(n);

  return info;
}

void info_destroy(search_info_t *info) {
  if (!info) return;

  free(info->depth);
  free(info->parent);
  free(info->time);
  free(info->preorder);
  free(info);
}

typedef struct edge {
  int source;
  int sink;
} edge_t;

typedef struct queue {
  edge_t *edges;
  int bottom;
  int top;
} queue_t;

static void push_edge(adj_graph_t *graph, int source, int sink, void *data)
{
  queue_t *queue = data;

  if (!queue || queue->top > adj_edge_count(graph))
    return;

  queue->edges[queue->top].source = source;
  queue->edges[queue->top].sink = sink;
  queue->top++;
}

static void generic_search(search_info_t *info, int root, bool use_queue)
{
  queue_t q;
  edge_t current;

  q.edges = malloc(sizeof(edge_t) * adj_edge_count(info->graph));
  if (!q.edges) return;

  q.bottom = q.top = 0;
  push_edge(info->graph, root, root, &q);

  while (q.bottom < q.top) {
    if (use_queue) {
      current = q.edges[q.bottom++];
    } else {
      current = q.edges[--q.top];
    }
    printf("%d -> %d\n", current.source, current.sink);

    if (info->parent[current.sink] != SEARCH_INFO_NULL) continue;
    if (info->reached > adj_vertex_count(info->graph)) break;

    info->parent[current.sink] = current.source;
    info->time[current.sink] = info->reached;
    info->preorder[info->reached++] = current.sink;

    if (current.source == current.sink) {
      info->depth[current.sink] = 0;
    } else {
      info->depth[current.sink] = info->depth[current.source] + 1;
    }

    adj_foreach(info->graph, current.sink, push_edge, &q);
  }

  free(q.edges);
}

void depth_first_search(search_info_t *info, int root)
{
  generic_search(info, root, false);
}

void breadth_first_search(search_info_t *info, int root)
{
  generic_search(info, root, true);
}

int main(int argc, char *argv[]) {
  adj_graph_t *graph;
  search_info_t *dfs_info, *bfs_info;

  graph = adj_create(16);
  adj_add_edge(graph, 0, 5);
  adj_add_edge(graph, 0, 2);
  adj_add_edge(graph, 2, 4);
  adj_add_edge(graph, 3, 4);
  adj_add_edge(graph, 4, 6);
  adj_add_edge(graph, 5, 0);
  adj_add_edge(graph, 6, 0);
  printf("Total graph: \n");
  adj_print_graph(graph);

  dfs_info = info_create(graph);
  printf("DFS from node 0: \n");
  depth_first_search(dfs_info, 0);

  bfs_info = info_create(graph);
  printf("BFS from node 0: \n");
  breadth_first_search(bfs_info, 0);

  info_destroy(bfs_info);
  info_destroy(dfs_info);
  adj_destroy(graph);

  return EXIT_SUCCESS;
}
