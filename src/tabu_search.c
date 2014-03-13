#include "hdr/tabu_search.h"

int compare(vertex_node_t* vertex_node1, vertex_node_t* vertex_node2){
    return vertex_node1->vertex->edges_size > 
        vertex_node2->vertex->edges_size;
}

/* O(2V) */
vertex_node_t** init_heap(Graph* graph, size_t* len){
    vertex_node_t** heapq = (vertex_node_t**) create_heap(graph->iterator_size);

    hash_iterator_t* j = graph->begin;
    while (j != NULL){
        heapq[*len] = (vertex_node_t*) j->hash_node->value;
        *len += 1;
        j = j->next;
    }

    build_heap((void**) heapq, *len, (int(*)(void*,void*)) &compare);

    return heapq;
}


void set_adj_colors(hash_table_t* adj_colors, Graph* graph_color, 
        vertex_node_t* vertex_node, size_t used_len){
    
    vertex_t* vertex = vertex_node->vertex;

    char color[10];
    unsigned int i;
    for (i = 0; i < vertex->edges_size; i++){
        vertex_node_t* adj_vertex = (vertex_node_t*) 
            get_vertex(graph_color, vertex->edges[i]);

        if (adj_vertex->color > -1){
            sprintf(color,"%d",adj_vertex->color);
            char* value = (char*) malloc(sizeof(char) *
                    strlen(vertex->name));
            strcpy(value, vertex->name);
            set_hash(adj_colors, color, value);
        }
    }


    for (i = 0; i < used_len; i++){
        sprintf(color,"%d",i);
        char* name = (char*) get_hash(adj_colors, color);

        if (name == NULL || strcmp(name, vertex->name) != 0){
            vertex_node->color = i;
            break;
        }
    }

}

/* Largest First Algorithm O(V*E) */
Graph* initial_solution(Graph* graph){
    Graph* solution = copy_graph(graph);

    size_t heapq_len = 0, used_len = 1;
    vertex_node_t** heapq = init_heap(solution, &heapq_len);

    hash_table_t* adj_colors = create_hash_table(heapq_len, &free);

    while (heapq_len > 0){
        vertex_node_t* vertex_node = (vertex_node_t*) heap_pop((void**) heapq, 
                &heapq_len, (int(*)(void*,void*)) &compare);

        set_adj_colors(adj_colors, solution, vertex_node, used_len);

        if (vertex_node->color == -1){
            vertex_node->color = used_len;
            used_len++;
        }

    }

    free(heapq);

    free_hash_table(adj_colors);

    return solution;
}

int cost(Graph* solution){
    hash_iterator_t* iter = solution->begin;
    int cost = 0;

    while (iter != NULL){
        vertex_node_t* vertex_node = (vertex_node_t*) iter->hash_node->value;
        if (cost < vertex_node->color + 1) cost = vertex_node->color + 1;
        iter = iter->next;
    }
    
    return cost;
}

Graph* generate_candidate(Graph** tabu_list, Graph* best,
        vertex_node_t** vertices, int cost){

    Graph* new_candidate = copy_graph(best);

    int vertices_size = best->iterator_size;

    srand(time(NULL));

    hash_table_t* adj_colors = create_hash_table(vertices_size, &free);

    while (vertices_size > 0){
        int sorted = (rand() % vertices_size) + 1;
        vertex_node_t* vertex_node = (vertex_node_t*) 
            get_hash(new_candidate, vertices[sorted]->vertex->name);
        
        int color = vertex_node->color;
        set_adj_colors(adj_colors, new_candidate, vertex_node, cost);
        if (color != vertex_node->color){
            printf("new: %d,  old: %d\n", color, vertex_node->color);
        }
        
        vertices_size--;
    }

    free_hash_table(adj_colors);

    return new_candidate;

}

Graph* tabu_search(Graph* graph){
    Graph* best = initial_solution(graph);
    int best_cost = cost(best);
    printf("%d\n", best_cost);
    vertex_node_t** vertices = get_vertices(graph);
    int iter = 0;
    int tabu_size = 10;
    Graph** tabu_list = (Graph**) malloc(sizeof(Graph*) * tabu_size);
    while (iter < 1000){
        Graph** candidate_list = (Graph**) malloc(sizeof(Graph*) * tabu_size);
        for (int i = 0; i < tabu_size; i++){
            candidate_list[i] = generate_candidate(tabu_list, best, 
                    vertices, best_cost);
        }
        for (int i = 0; i < tabu_size; i++){
            int cost_value = cost(candidate_list[i]);
            free_graph(candidate_list[i]);
        }
        free(candidate_list);
        iter++;
    }
    free(tabu_list);
    return best;
}
