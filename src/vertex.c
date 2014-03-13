#include <string.h>
#include <stdlib.h>
#include "hdr/vertex.h"

vertex_t* create_vertex_t(char* name){
    if (strlen(name) == 0) return NULL;
    vertex_t* vertex = (vertex_t*) malloc(sizeof(vertex_t));
    vertex->name = (char*) malloc(sizeof(char) * strlen(name));
    strcpy(vertex->name, name);
    vertex->edges_size = 0;
    return vertex;
}

void* set_edge(vertex_t* vertex, char* edge){
    if (vertex == NULL || strlen(edge) == 0) return NULL;
    if (vertex->edges_size == 0){
        vertex->edges = (char**) malloc(sizeof(char*));
        vertex->edges[0] = (char*) malloc(sizeof(char) * strlen(edge));
        strcpy(vertex->edges[0], edge);
    }
    else{
        char** edges = (char**) realloc(vertex->edges, 
                sizeof(char*) * vertex->edges_size + 1);
        if (edges != NULL){
            vertex->edges = edges;
            vertex->edges[vertex->edges_size] = (char*) malloc(sizeof(char) * 
                    strlen(edge));
            strcpy(vertex->edges[vertex->edges_size], edge);
        }
        else return NULL;
    }
    vertex->edges_size++;
    return (void*) 1;
}

vertex_node_t* create_vertex_node(vertex_t* vertex){
    vertex_node_t* vertex_node = (vertex_node_t*) 
        malloc(sizeof(vertex_node_t));

    vertex_node->vertex = vertex;
    vertex_node->color = -1;

    return vertex_node;
}

void free_vertex_node(vertex_node_t* vertex_node){
    free_vertex_t(vertex_node->vertex);
    free(vertex_node);
}

void free_vertex_t(vertex_t* vertex){
    free(vertex->name);
    unsigned int i;
    for (i = 0; i < vertex->edges_size; i++)
        free(vertex->edges[i]);
    free(vertex->edges);
    free(vertex);
}
