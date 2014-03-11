#include <stdlib.h>
#include "hdr/graph.h"

Graph* create_graph(){
    return create_hash_table(64000);
}

void read_file_on_graph(Graph* graph, char* file){
    char type = get_file_type(file);

    if (type == 'a') 
        read_ascii(file, graph);
    else if (type == 'b')
        write_graph_DIMACS_ascii(file, graph, 
                (void(*)(void*,int,int)) &populate_graph);

}

void set_vertex(Graph* graph, char* key, vertex_node_t* vertex){
    set_hash(graph, key, (void*) vertex);
}

vertex_node_t* get_vertex(Graph* graph, char* key){
    return (vertex_node_t*) get_hash(graph, key);
}

void free_graph(Graph* graph){
    free_hash_table(graph, (void (*)(void*)) &free_vertex_node);
}

void populate_graph(Graph* graph, int a, int b){
    char edge[2][10];
    sprintf(edge[0], "%d", a);
    sprintf(edge[1], "%d", b);
    int i;
    for (i = 0; i < 2; i++){
        vertex_node_t* vertex = get_vertex(graph, edge[i]);
        if (vertex == NULL){
            vertex = create_vertex_node(edge[i]);
            set_edge(vertex, edge[(i + 1) % 2]);
            set_vertex(graph, edge[i], vertex);
        }
        else set_edge(vertex, edge[(i + 1) % 2]);
    }
}

Graph* graph_copy(Graph* table){
    Graph* table_copy = create_hash_table(table->size);
    hash_iterator_t* iter = table->begin;
    
    while (iter != NULL){
        vertex_node_t* vertex = (vertex_node_t*) iter->hash_node->value;
        vertex_node_t* value  = (vertex_node_t*) malloc(sizeof(vertex_node_t));
        
        value->name = (char*) malloc(sizeof(char) * strlen(vertex->name));
        strcpy(value->name, vertex->name);
        
        value->color = vertex->color;
        value->edges_size = vertex->edges_size;
        
        value->edges = (char**) malloc(sizeof(char*) * value->edges_size);
        for (unsigned int i = 0; i < value->edges_size; i++){
            value->edges[i] = (char*) malloc(sizeof(char) * 
                    strlen(vertex->edges[i]));
            strcpy(value->edges[i], vertex->edges[i]);
        }
        
        set_hash(table_copy, value->name, value);
        iter = iter->next;
    }
    
    return table_copy;
}

void read_ascii(char* filename, Graph* graph){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("File does not exist!\n");
        exit(1);
    }
    else{
        char line[255], type;
        int a, b;
        while (fgets(line,255,file) != NULL){
            sscanf(line,"%c",&type);
            if (type == 'e'){
                sscanf(line,"%*s %d %d", &a, &b);
                populate_graph(graph, a, b);
            }
        }
    }
}

char get_file_type(char* file){
    unsigned int i;
    for (i = 0; i < strlen(file) - 1; i++){
        if (file[i] == '.' && file[i + 1] == 'b')
            return 'b';
    }
    return 'a';
}
