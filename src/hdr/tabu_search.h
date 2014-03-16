#ifndef TABU_H
#define TABU_H

#include "heap.h"
#include "graph.h"
#include "hash.h"
#include "vertex.h"
#include <time.h>

#define MAX_ITER 1000

void set_adj_colors(hash_table_t*, Graph*, vertex_t*);
void set_min_color(hash_table_t*, vertex_node_t*, size_t*);
Graph* tabu_search(Graph*, int, int);

#endif
