#include "graph.h"
#include "read.h"
#include "heap.h"

/* < for heap min, > for heap max */
int compare(vertex_node_t* vertex1, vertex_node_t* vertex2){
    return vertex1->edges_size > vertex2->edges_size;
}

int main(int argc, char** argv){
    if (argc > 2){
        vertex_node_t** heapq = (vertex_node_t**) create_heap(1024);
        size_t heapq_len = 0;
        
        Graph* graph = create_graph();
        read_file(graph, argv[1]);

        hash_iterator_t* j = graph->begin;
        while (j != NULL){
            heapq[heapq_len++] = j->hash_node->value;
            j = j->next;
        }
        
        build_heap((void**) heapq, heapq_len, (int(*)(void*,void*)) &compare);

        vertex_node_t* vertex = heap_find((void**) heapq);
        printf("%s %d\n", vertex->name, vertex->edges_size);

        vertex = get_vertex(graph, argv[2]);
        if (vertex == NULL) return 0;
        printf("%s: ", vertex->name);
        int i;
        for (i = 0; i < vertex->edges_size; i++){
            printf("%s | ", vertex->edges[i]);
        }
        printf("\n");
        free_graph(graph);
    }
    else printf("Missing arguments.\n");
    return 0;
}
