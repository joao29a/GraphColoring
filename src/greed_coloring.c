#include "greed_coloring.h"

int compare(vertex_node_t* vertex1, vertex_node_t* vertex2){
    return vertex1->edges_size > vertex2->edges_size;
}

/* this shit is ugly as fuck, but it works hard as fuck */
/* Greedy Algorithm + LDO */
size_t greed_coloring(Graph* graph){
    vertex_node_t** heapq = (vertex_node_t**) create_heap(1024);
    size_t heapq_len = 0;
    
    hash_iterator_t* j = graph->begin;
    while (j != NULL){
        heapq[heapq_len++] = j->hash_node->value;
        j = j->next;
    }

    build_heap((void**) heapq, heapq_len, (int(*)(void*,void*)) &compare);

    size_t used_len = 0;
    hash_table_t* adj_colors = create_hash_table(heapq_len);
    char color[20];

    while (heapq_len != 0){
        vertex_node_t* vertex = heap_pop((void**) heapq, &heapq_len, 
                (int(*)(void*,void*)) &compare);
        
        int i, empty = 1;
        for (i = 0; i < vertex->edges_size; i++){
            vertex_node_t* adj_vertex = get_vertex(graph, vertex->edges[i]);
            if (adj_vertex->color > -1){
                empty = 0;
                sprintf(color,"%d",adj_vertex->color);
                set_hash(adj_colors, color, vertex->name);
            }
        }

        if (empty){
            vertex->color = 0;
            if (used_len == 0) used_len++;
            continue;
        }

        for (i = 0; i < used_len; i++){
            sprintf(color,"%d",i);
            hash_node_t* adj_color = get_hash(adj_colors, color);

            if (adj_color == NULL){
                vertex->color = i;
                break;
            }
            else if (strcmp((char*) adj_color, vertex->name) != 0){
                vertex->color = i;
                break;
            }
        }

        if (i == used_len){
            vertex->color = used_len;
            used_len++;
        }

    }
    return used_len;
}
