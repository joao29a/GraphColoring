#include "read.h"

void* read_file(Graph* graph, char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("File does not exist!\n");
        return NULL;
    }
    else{
        char line[255], type, key[2][50];
        while (fgets(line,255,file) != NULL){
            sscanf(line,"%c",&type);
            if (type == 'e'){
                sscanf(line,"%*s %s %s",key[0], key[1]);

                int i;
                for (i = 0; i < 2; i++){
                    vertex_node_t* vertex = get_vertex(graph, key[i]);

                    if (vertex == NULL){
                        vertex = create_vertex_node(key[i]);
                        set_edge(vertex, key[(i + 1) % 2]);
                        set_vertex(graph, key[i], vertex);
                    }
                    else set_edge(vertex, key[(i + 1) % 2]);
                }
            }
        }
    }
    return (void*) 1;
}
