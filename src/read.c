#include "read.h"

void read_file(Graph* graph, int a, int b){
    char edge[2][50];
    sprintf(edge[0],"%d",a);
    sprintf(edge[1],"%d",b);
    
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
