#ifndef TABU_H
#define TABU_H

#include "heap.h"
#include "graph.h"
#include "hash.h"
#include "vertex.h"
#include <time.h>

void set_adj_colors(hash_table_t*, Graph*, vertex_node_t*, size_t);
Graph* tabu_search(Graph*);

#endif
