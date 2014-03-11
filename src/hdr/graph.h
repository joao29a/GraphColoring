#ifndef GRAPH_H
#define GRAPH_H

#include <string.h>
#include "hash.h"
#include "vertex.h"
#include "bin2asc.h"

typedef hash_table_t Graph;

Graph* create_graph();
void read_file_on_graph(Graph*, char*);
void read_ascii(char*, Graph*);
Graph* graph_copy(Graph*);
void populate_graph(Graph*, int, int);
void set_vertex(Graph*, char*, vertex_node_t*);
vertex_node_t* get_vertex(Graph*, char*);
void free_graph(Graph*);
char get_file_type(char*);

#endif
