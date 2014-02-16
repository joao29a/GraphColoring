#include "graph.h"
#include "read.h"
#include "heap.h"
#include "greed_coloring.h"

int main(int argc, char** argv){
    if (argc > 1){
        Graph* graph = create_graph();
        if (read_file(graph, argv[1]) == NULL) return 0;
        size_t total_colors = greed_coloring(graph);
        hash_iterator_t* j = graph->begin;
        while (j != NULL){
            vertex_node_t* node = (vertex_node_t*) j->hash_node->value;
            printf("%s: %d\n", j->hash_node->key, node->color);
            j = j->next;
        }
        printf("total colors: %zu\n", total_colors);

        free_graph(graph);
    }
    else printf("Missing arguments.\n");
    return 0;
}
