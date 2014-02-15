#include "graph.h"

Graph* create_graph(){
    return create_hash_table(64000);
}

void set_vertex(Graph* graph, char* key, vertex_node_t* vertex){
   set_hash(graph, key, (void*) vertex);
}

vertex_node_t* get_vertex(Graph* graph, char* key){
    return get_hash(graph, key);
}

void free_graph(Graph* graph){
    free_hash_table(graph, &free_vertex_node);
}
