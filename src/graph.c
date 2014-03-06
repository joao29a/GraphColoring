#include <stdlib.h>
#include "hdr/graph.h"

Graph* create_graph(char* file){
    Graph* graph = create_hash_table(64000);

    char type = get_file_type(file);

    if (type == 'a') 
        read_ascii(file, graph);
    else if (type == 'b')
        write_graph_DIMACS_ascii(file, graph, 
                (void(*)(void*,int,int)) &populate_graph);

    return graph;
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
    char edge[2][50];
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
