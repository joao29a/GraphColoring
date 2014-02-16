#include <string.h>
#include <stdlib.h>
#include "vertex.h"

vertex_node_t* create_vertex_node(char* name){
    if (strlen(name) == 0) return NULL;
    vertex_node_t* vertex = malloc(sizeof(vertex_node_t));
    vertex->name =malloc(sizeof(char) * strlen(name));
    strcpy(vertex->name, name);
    vertex->color = -1;
    vertex->edges_size = 0;
    return vertex;
}

void* set_edge(vertex_node_t* vertex, char* edge){
    if (vertex == NULL || strlen(edge) == 0) return NULL;
    if (vertex->edges_size == 0){
        vertex->edges = malloc(sizeof(char*));
        vertex->edges[0] = malloc(sizeof(char) * strlen(edge));
        strcpy(vertex->edges[0], edge);
    }
    else{
        char** edges = realloc(vertex->edges, 
                sizeof(char*) * vertex->edges_size + 1);
        if (edges != NULL){
            vertex->edges = edges;
            vertex->edges[vertex->edges_size] = malloc(sizeof(char) * 
                    strlen(edge));
            strcpy(vertex->edges[vertex->edges_size], edge);
        }
        else return NULL;
    }
    vertex->edges_size++;
    return (void*) 1;
}

void free_vertex_node(vertex_node_t* vertex){
    free(vertex->name);
    unsigned int i;
    for (i = 0; i < vertex->edges_size; i++)
        free(vertex->edges[i]);
    free(vertex->edges);
    free(vertex);
}
