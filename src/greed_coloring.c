#include "greed_coloring.h"

int compare(vertex_node_t* vertex1, vertex_node_t* vertex2){
    return vertex1->edges_size > vertex2->edges_size;
}

/* O(2V) */
vertex_node_t** largest_degree_ordering(Graph* graph, size_t* len){
    vertex_node_t** heapq = (vertex_node_t**) create_heap(1024);
    
    hash_iterator_t* j = graph->begin;
    while (j != NULL){
        heapq[*len] = j->hash_node->value;
        *len += 1;
        j = j->next;
    }

    build_heap((void**) heapq, *len, (int(*)(void*,void*)) &compare);

    return heapq;
}

/* Greedy Algorithm + LDO  O(V*E)  */
size_t greed_coloring(Graph* graph){
    size_t heapq_len = 0, used_len = 1;
    vertex_node_t** heapq = largest_degree_ordering(graph, &heapq_len);

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
        for (i = 0; i < used_len; i++){
            sprintf(color,"%d",i);
            hash_node_t* adj_color = get_hash(adj_colors, color);
            
            if (adj_color == NULL || 
                    strcmp((char*) adj_color, vertex->name) != 0){
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
