#include "read.h"

void* read_file(Graph* graph, char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("File does not exist!\n");
        return NULL;
    }
    else{
        char line[255], type;
        while (fgets(line,255,file) != NULL){
            sscanf(line,"%c",&type);
            if (type == 'e'){
                char* key = malloc(sizeof(char)*50);
                char* value = malloc(sizeof(char)*50);
                sscanf(line,"%*s %s %s", key, value);

                vertex_node_t* vertex = get_vertex(graph, key);

                if (vertex == NULL){
                    vertex = create_vertex_node(key);
                    set_edge(vertex, value);
                    set_vertex(graph, key, vertex);
                }
                else set_edge(vertex, value);
                
                free(key);
                free(value);
            }
        }
    }
    return (void*) 1;
}
