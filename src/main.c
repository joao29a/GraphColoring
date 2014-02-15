#include "graph.h"
#include "read.h"

int main(int argc, char** argv){
    if (argc > 2){
        Graph* graph = create_graph();
        read_file(graph, argv[1]);
        vertex_node_t* vertex = get_vertex(graph, argv[2]);
        if (vertex == NULL) return 0;
        int i;
        printf("%s: ", vertex->name);
        for (i = 0; i < vertex->edges_size; i++){
            printf("%s | ", vertex->edges[i]);
        }
        free_graph(graph);
    }
    else printf("Missing arguments.\n");
    return 0;
}
