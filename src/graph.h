#ifndef GRAPH_H
#define GRAPH_H

#include "hash.h"
#include "vertex.h"

typedef hash_table_t Graph;

Graph* create_graph();
void set_vertex(Graph*, char*, vertex_node_t*);
vertex_node_t* get_vertex(Graph*, char*);
void free_graph(Graph*);

#endif
