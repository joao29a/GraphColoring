#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex_node {
    char* name;
    int color;
    unsigned int edges_size;
    char** edges;

} vertex_node_t;

vertex_node_t* create_vertex_node(char*);
void* set_edge(vertex_node_t*,char*);
void free_vertex_node(vertex_node_t*);

#endif
