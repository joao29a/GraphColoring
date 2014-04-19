#include "../hdr/graph.h"
#include "../hdr/bin2asc.h"

void check_result(Graph* graph){
    char line[1024];
    char vertex[1024];
    int color, total_colors = 0;
    while (fgets(line, 1024, stdin) != NULL){
        sscanf(line,"%s %d", vertex, &color);
        vertex_node_t* vertex_node = get_vertex(graph, vertex);
        vertex_node->color = color;
        if (color > total_colors) total_colors = color;
    }

    hash_iterator_t* iter = graph->begin;

    while (iter != NULL){
        vertex_node_t* vertex_node = (vertex_node_t*) iter->hash_node->value;

        for (u_int32 i = 0; i < vertex_node->vertex->edges_size; i++){
            vertex_node_t* adj_vertex_node = 
                get_vertex(graph, vertex_node->vertex->edges[i]);

            if (vertex_node->color == adj_vertex_node->color){
                printf("Fail. %s %s ", vertex_node->vertex->name,
                        adj_vertex_node->vertex->name);
                return;
            }
        }

        iter = iter->next;
    }
    printf("OK. Colors: %d ", total_colors + 1);
}


int main(int argc, char** argv){
    if (argc > 1){
        Graph *graph;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);
        check_result(graph);
    }
    else printf("Insert file name.\n");
    return 0;
}
