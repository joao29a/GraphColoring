#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    char* name;
    unsigned int edges_size;
    char** edges;
} vertex_t;

typedef struct vertex_node {
    vertex_t* vertex;
    int color;
}vertex_node_t;

vertex_t* create_vertex_t(char*);
vertex_node_t* create_vertex_node(vertex_t*);
void* set_edge(vertex_t*,char*);
void free_vertex_node(vertex_node_t*);
void free_vertex_t(vertex_t*);

#endif
