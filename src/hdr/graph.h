#ifndef GRAPH_H
#define GRAPH_H

#include <string.h>
#include "hash.h"
#include "vertex.h"
#include "bin2asc.h"

typedef hash_table_t Graph;

Graph* copy_graph(Graph*);

Graph* create_graph();
void read_file_on_graph(Graph*, char*);
void read_ascii(char*, Graph*);
void free_graph(Graph*);
vertex_node_t** get_vertices(Graph*);
void populate_graph(Graph*, int, int);
void set_vertex(Graph*, char*, vertex_node_t*);
vertex_node_t* get_vertex(Graph*, char*);
char get_file_type(char*);

#endif
