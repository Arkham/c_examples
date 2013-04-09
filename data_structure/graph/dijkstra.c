#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#define NUM_ELEM 16
#define GRAPH_DEFAULT_WEIGHT 1
#define DIJKSTRA_NULL_PARENT (-1)

typedef struct edge {
  int sink;
  int weight;
} edge_t;

typedef struct adj_list {
  int count; // number of elements
  int length; // size of array
  bool is_sorted;
  edge_t edges[1];
} adj_list_t;

typedef struct graph {
  int vertices;
  int edges;
  adj_list_t * adj_list[1];
} graph_t;

graph_t *graph_create(int size) {
  graph_t *graph;
  int i;

  graph = malloc(sizeof(graph_t) + sizeof(adj_list_t *) * (size - 1));
  if (!graph) return NULL;

  graph->vertices = 0;
  graph->edges = 0;

  for (i=0; i<size; i++) {
    graph->adj_list[i] = malloc(sizeof(adj_list_t));
    if (! graph->adj_list[i]) return NULL;

    graph->adj_list[i]->count = 0;
    graph->adj_list[i]->length = 1;
    graph->adj_list[i]->is_sorted = true;

    graph->vertices++;
  }

  return graph;
}

void graph_destroy(graph_t *graph) {
  int i;

  if (!graph) return;

  for (i=0; i<graph->vertices; i++) {
    free(graph->adj_list[i]);
  }
  free(graph);
}

void graph_add_weighted_edge(graph_t *graph, int source, int sink, int weight) {
  adj_list_t *list;

  if (!graph ||
      source < 0 || source >= graph->vertices ||
      sink < 0 || sink >= graph->vertices ||
      !graph->adj_list[source])
    return;

  list = graph->adj_list[source];

  while ( list->count >= list->length ) {
    list->length *= 2;
    list = graph->adj_list[source] =
      realloc(graph->adj_list[source],
          sizeof(adj_list_t) + sizeof(edge_t) * (list->length - 1));
  }

  list->edges[list->count].sink = sink;
  list->edges[list->count].weight = weight;
  list->is_sorted = false;
  list->count++;

  graph->edges++;
}

void graph_add_edge(graph_t *graph, int source, int dest) {
  graph_add_weighted_edge(graph, source, dest, GRAPH_DEFAULT_WEIGHT);
}

int graph_vertex_count(graph_t *graph) {
  return graph ? graph->vertices : -1;
}

int graph_edge_count(graph_t * graph) {
  return graph ? graph->edges : -1;
}

int graph_out_degree(graph_t *graph, int source) {
  if (graph && source >= 0 && source < graph->vertices && graph->adj_list[source])
    return graph->adj_list[source]->count;
  else
    return -1;
}

#define BSEARCH_THRESHOLD 10

static int edge_cmp(const void *first, const void *second)
{
  return ((edge_t *) first)->sink - ((edge_t *) second)->sink;
}

/* return true if edge (source, sink) exists, false otherwise */
static bool graph_edge_exists_internal(graph_t *graph, int source, int sink, int *weight)
{
  int i;
  adj_list_t *list;
  edge_t sink_edge;
  edge_t *bsearch_result;

  if (!graph ||
      source < 0 || source >= graph->vertices ||
      sink < 0 || sink >= graph->vertices ||
      !graph->adj_list[source])
    return -1;

  if (weight) *weight = INT_MAX;
  list = graph->adj_list[source];

  if (graph_out_degree(graph, source) >= BSEARCH_THRESHOLD) {
    /* qsort && bsearch */
    if (! list->is_sorted) {
      qsort(list, list->count, sizeof(edge_t), edge_cmp);
    }

    sink_edge.sink = sink;
    sink_edge.weight = GRAPH_DEFAULT_WEIGHT;

    bsearch_result =
      bsearch(&sink_edge,
          list->edges, list->count, sizeof(edge_t), edge_cmp);

    if (bsearch_result != 0) {
      if (weight) *weight = bsearch_result->weight;
      return true;
    } else {
      return false;
    }


  } else {
    for (i=0; i<list->count; i++) {
      if (list->edges[i].sink == sink) {
        if (weight) *weight = list->edges[i].weight;
        return true;
      }
    }

    return false;
  }
}

bool graph_edge_exists(graph_t *graph, int source, int sink)
{
  return graph_edge_exists_internal(graph, source, sink, NULL);
}

int graph_edge_weight(graph_t *graph, int source, int sink)
{
  int weight;

  if (graph_edge_exists_internal(graph, source, sink, &weight))
    return weight;
  else
    return -1;
}

void graph_print_edges(graph_t *graph) {
  int i, j;
  adj_list_t * list;

  if (!graph) return;
  for (i=0; i<graph->vertices; i++) {
    list = graph->adj_list[i];
    for (j=0; j<list->count; j++) {
      printf("%d -> %d: %d\n", i, list->edges[j].sink, list->edges[j].weight);
    }
  }
}

void graph_foreach(graph_t *graph, int source, void (*f) (graph_t *graph, int source, int sink, void *data), void *data)
{
  int i;
  adj_list_t *list;

  if (!graph || source < 0 || source >= graph->vertices || ! graph->adj_list[source] )
    return;

  list = graph->adj_list[source];

  for (i=0; i<list->count; i++) {
    f(graph, source, list->edges[i].sink, data);
  }
}

void graph_foreach_weighted(graph_t *graph, int source, void (*f) (graph_t *graph, int source, int sink, int weight, void *data), void *data)
{
  int i;
  adj_list_t *list;

  if (!graph || source < 0 || source >= graph->vertices || ! graph->adj_list[source] )
    return;

  list = graph->adj_list[source];

  for (i=0; i<list->count; i++) {
    f(graph, source, list->edges[i].sink, list->edges[i].weight, data);
  }
}

void graph_print_edge_weight(graph_t *graph, int source, int sink, int weight, void *data)
{
  printf("edge: %d -> %d: %d\n", source, sink, weight);
}

struct pq {
  int element_length;
  int (*compare)(const void *, const void *);
  int n;      /* number of elements */
  int size;   /* number of slots in data */
  void *swap_space;   /* element_length bytes used for swapping */
  void *data;
};
typedef struct pq *PQ;

#define PQ_INITIAL_SIZE (128)

PQ
pq_create(int element_length, int (*compare)(const void *, const void *))
{
  PQ pq;

  pq = malloc(sizeof(*pq));
  assert(pq);

  pq->element_length = element_length;
  pq->compare = compare;
  pq->n = 0;
  pq->size = PQ_INITIAL_SIZE;

  pq->swap_space = malloc(pq->element_length);
  assert(pq->swap_space);

  pq->data = malloc(pq->element_length * pq->size);
  assert(pq->data);

  return pq;
}

void
pq_destroy(PQ pq)
{
  free(pq->data);
  free(pq->swap_space);
  free(pq);
}

  int
pq_is_empty(PQ pq)
{
  return pq->n == 0;
}

/* Child(i, 0) and Child(i, 1) are children of i */
/* Parent(i) is parent of i */
#define Child(i, x) (2*(i)+1+(x))
#define Parent(i) (((i)-1)/2)

#define NUM_CHILDREN (2)

/* compute the address of position i in the data field */
#define REF(pq, i) ((void *) (((char *) (pq)->data) + (pq)->element_length * i))

/* swap elements at indexes i1 and i2 */
static void
pq_swap(PQ pq, int i1, int i2)
{
  memcpy(pq->swap_space, REF(pq, i1), pq->element_length);
  memcpy(REF(pq, i1), REF(pq, i2), pq->element_length);
  memcpy(REF(pq, i2), pq->swap_space, pq->element_length);
}

void
pq_insert(PQ pq, const void *elt)
{
  int floater;                /* new element */

  while(pq->n + 1 > pq->size) {
    pq->size *= 2;
    pq->data = realloc(pq->data, pq->element_length * pq->size);
    assert(pq->data);
  }

  /* copy the new element in */
  floater = pq->n++;
  memcpy(REF(pq, floater), elt, pq->element_length);

  /* float it up until it is at the top */
  /* or it is no smaller than its parent */
  while(floater > 0 && pq->compare(REF(pq, floater), REF(pq, Parent(floater))) <= 0) {
    /* it's smaller than its parent */
    pq_swap(pq, floater, Parent(floater));
    floater = Parent(floater);
  }
}

void
pq_delete_min(PQ pq, void *retval)
{
  int floater;        /* previous loser floating down */
  int small_child;    /* smaller child of floater */

  assert(!pq_is_empty(pq));

  /* first copy out the winner */
  memcpy(retval, REF(pq, 0), pq->element_length);

  --(pq->n);

  if(pq_is_empty(pq)) {
    /* pq empty, nothing to do */
    return;
  }

  /* else */
  memcpy(REF(pq, 0), REF(pq, pq->n), pq->element_length);

  floater = 0;

  for(;;) {
    /* find smaller child of floater */
    if(Child(floater, 0) >= pq->n) {
      return;     /* no children, bail out */
    } else if(Child(floater, 1) >= pq->n) {
      small_child = Child(floater, 0);
    } else if(pq->compare(REF(pq, Child(floater, 0)), REF(pq, Child(floater, 1))) < 0) {
      small_child = Child(floater, 0);
    } else {
      small_child = Child(floater, 1);
    }

    /* is floater <= small_child? */
    if(pq->compare(REF(pq, floater), REF(pq, small_child)) <= 0) {
      /* yes, we are done */
      return;
    } else {
      /* no, swap and continue floating down */
      pq_swap(pq, floater, small_child);
      floater = small_child;
    }
  }
}

void
pq_sanity_check(PQ pq)
{
  int i;
  int j;

  assert(pq->n >= 0);
  assert(pq->n <= pq->size);

  for(i = 0; i < pq->n; i++) {
    for(j = 0; j < NUM_CHILDREN; j++) {
      if(Child(i, j) < pq->n) {
        assert(pq->compare(REF(pq, i), REF(pq, Child(i, j))) <= 0);
      }
    }
  }
}

typedef struct pq_elem {
  int distance;
  int source;
  int sink;
} pq_elem_t;

static int pq_elem_compare(const void *first, const void *second)
{
  return ((pq_elem_t *) first)->distance - ((pq_elem_t *) second)->distance;
}

typedef struct push_data {
  PQ pq;
  int *dist;
} push_data_t;

static void push(graph_t *graph, int source, int sink, int weight, void *data)
{
  push_data_t *d;
  pq_elem_t e;

  d = data;

  e.distance = d->dist[source] + weight;
  e.source = source;
  e.sink = sink;

  pq_insert(d->pq, &e);
}

void dijkstra(graph_t *graph, int source, int *dist, int *parent)
{
  push_data_t data;
  pq_elem_t e;
  int n, i;

  if (!dist) return;

  data.dist = dist;
  data.pq = pq_create(sizeof(pq_elem_t), pq_elem_compare);
  if (!data.pq) return;

  n = graph_vertex_count(graph);

  for (i=0; i<n; i++)
    dist[i] = INT_MAX;

  if (parent) {
    for (i=0; i<n; i++) {
      parent[i] = DIJKSTRA_NULL_PARENT;
    }
  }

  push(graph, source, source, -INT_MAX, &data);

  while(! pq_is_empty(data.pq)) {
    pq_delete_min(data.pq, &e);
    printf("%d -> %d: %d\n", e.source, e.sink, e.distance);

    if (dist[e.sink] == INT_MAX) {
      /* new edge */
      dist[e.sink] = e.distance;
      if (parent)
        parent[e.sink] = e.source;

      graph_foreach_weighted(graph, e.sink, push, &data);
    }
  }

  pq_destroy(data.pq);
}

int main(int argc, char *argv[]) {
  graph_t *graph;
  int i, n, *dist, *parent;

  graph = graph_create(NUM_ELEM);
  graph_add_weighted_edge(graph, 0, 9, 200);
  graph_add_edge(graph, 0, 2);
  graph_add_edge(graph, 0, 4);
  graph_add_edge(graph, 0, 6);
  graph_add_edge(graph, 2, 4);
  graph_add_edge(graph, 2, 7);
  graph_add_edge(graph, 7, 9);
  printf("\nAll edges: \n");
  graph_print_edges(graph);

  printf("\nAll edges from 0: \n");
  graph_foreach_weighted(graph, 0, &graph_print_edge_weight, NULL);

  printf("\nDijkstra: \n");
  n = graph_vertex_count(graph);
  dist = malloc(sizeof(int) * n);
  parent = malloc(sizeof(int) * n);

  dijkstra(graph, 0, dist, parent);

  for (i=0; i<n; i++)
    if (dist[i] == INT_MAX)
      printf(" no ");
    else
      printf("%3d ", dist[i]);
  printf("\n");
  for (i=0; i<n; i++)
    printf("%3d ", parent[i]);
  printf("\n");

  graph_destroy(graph);

  return EXIT_SUCCESS;
}
