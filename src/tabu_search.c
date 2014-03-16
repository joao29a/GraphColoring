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
        vertex_t* vertex){

    char color[10];
    for (u_int32 i = 0; i < vertex->edges_size; i++){
        vertex_node_t* adj_vertex = (vertex_node_t*) 
            get_vertex(graph_color, vertex->edges[i]);

        if (adj_vertex->color != -1){
            sprintf(color,"%d",adj_vertex->color);
            char* value = (char*) malloc(sizeof(char) * strlen(vertex->name));
            strcpy(value, vertex->name);
            set_hash(adj_colors, color, value);
        }
    }

}

void set_min_color(hash_table_t* adj_colors, vertex_node_t* vertex_node, 
        size_t* used_len){

    char color[10];
    for (u_int32 i = 0; i < *used_len; i++){
        sprintf(color,"%lu",i);
        char* name = (char*) get_hash(adj_colors, color);

        if (name == NULL || strcmp(name, vertex_node->vertex->name) != 0){
            vertex_node->color = i;
            break;
        }
    }

    if (vertex_node->color == -1){
        vertex_node->color = *used_len;
        *used_len += 1;
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

        vertex_t* vertex = vertex_node->vertex;

        set_adj_colors(adj_colors, solution, vertex);

        set_min_color(adj_colors, vertex_node, &used_len);

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

Graph* reduce_color(Graph* graph, int cost){
    Graph* s = copy_graph(graph);
    
    hash_iterator_t* iter = s->begin;
    
    while (iter != NULL){
        vertex_node_t* vertex_node = (vertex_node_t*) iter->hash_node->value;
        if (vertex_node->color == (cost - 1)){
            vertex_node->color = rand() % (cost - 2);
        }
        iter = iter->next;
    }

    return s;
}

int conflict_cost(Graph* graph, char** conflict_vertices){
    hash_iterator_t* iter = graph->begin;
    
    int cost = 0;

    while (iter != NULL){
        vertex_node_t* vertex_node = (vertex_node_t*) iter->hash_node->value;
        vertex_t* vertex = vertex_node->vertex;

        for (u_int32 i = 0; i < vertex->edges_size; i++){
            vertex_node_t* adj_vertex_node = get_vertex(graph, vertex->edges[i]);

            if (vertex_node->color == adj_vertex_node->color){
                if (conflict_vertices != NULL){
                    conflict_vertices[cost] = vertex_node->vertex->name;
                }
                cost++;
            }
        }

        iter = iter->next;
    }

    return cost;
}

vertex_node_t** malloc_vertex_arr(int size){
    return (vertex_node_t**) malloc(sizeof(vertex_node_t*) * size);
}

Graph* generate_candidates(Graph* graph, char** conflict_vertices,
        int conflict_c, vertex_node_t** tabu_list, int* tabu_pos, 
        char** vertices){

    Graph* candidate = copy_graph(graph);

    char* vertex_name = conflict_vertices[rand() % conflict_c];

    vertex_node_t* vertex_node = get_vertex(candidate, vertex_name);

    vertex_name = vertices[rand() % graph->iterator_size];

    vertex_node_t* vertex_node2 = get_vertex(candidate, vertex_name);
    
    int tmp = vertex_node->color;
    vertex_node->color = vertex_node2->color;
    vertex_node2->color = tmp;

    return candidate;
}

Graph* get_best_candidate(Graph** candidates, int candidates_len, int* best_cost){
    Graph* best = candidates[0];
    int conflict_best = conflict_cost(candidates[0], NULL);
    for (int i = 1; i < candidates_len; i++){
        int conflict_c = conflict_cost(candidates[i], NULL);
        if (conflict_best > conflict_c){
            free_graph(best);
            best = candidates[i];
            conflict_best = conflict_c;
        }
        else free_graph(candidates[i]);
    }
    *best_cost = conflict_best;
    return best;
}

Graph* tabu_search(Graph* graph, int tabu_len, int candidates_len){
    Graph* best = initial_solution(graph);
    int best_cost = cost(best);

    srand(time(NULL));

    printf("Initial: %d\n", best_cost);

    char** vertices = get_vertices(graph);

    Graph* s = reduce_color(best, best_cost);

    char** conflict_vertices = (char**) malloc(sizeof(char*) * best->iterator_size);

    int conflict_c = conflict_cost(s, conflict_vertices);

    int tabu_pos = 0;
    vertex_node_t** tabu_list = malloc_vertex_arr(tabu_len);

    int iter = 0;

    while (iter < MAX_ITER){
        Graph** candidates = (Graph**) malloc(sizeof(Graph*) * candidates_len);
        for (int i = 0; i < candidates_len; i++){
            candidates[i] = generate_candidates(s, conflict_vertices, conflict_c,
                    tabu_list, &tabu_pos, vertices);
        }

        int conflict_cand_c;
        Graph* best_candidate = get_best_candidate(candidates, candidates_len, 
                &conflict_cand_c);

        if (conflict_c > conflict_cand_c){
            free_graph(s);
            s = best_candidate;
            conflict_c = conflict_cost(s, conflict_vertices);
        }
        else free_graph(best_candidate);

        if (conflict_c == 0){
            best_cost = cost(s);
            free_graph(best);
            best = s;

            s = reduce_color(best, best_cost);
            conflict_c = conflict_cost(s, conflict_vertices);
        }

        free(candidates);
        iter++;
    }

    free(vertices);
    free(conflict_vertices);
    free(tabu_list);

    printf("cost: %d\n",best_cost);

    return best;
}
