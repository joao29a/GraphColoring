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
        if (vertex_node->color == cost){
            vertex_node->color = rand() % cost;
        }
        iter = iter->next;
    }

    return s;
}


void increment_cost(char* name, char* vertex_name, hash_table_t* rep, int* cost, 
        char** conflict_vt){
    if (name == NULL){
        set_hash(rep, vertex_name, vertex_name);
        if (conflict_vt != NULL)
            conflict_vt[*cost] = vertex_name;
        *cost += 1;
    }
}

void count_cost(Graph* graph, char* name, hash_table_t* rep, int* cost,
        char** conflict_vt){
    vertex_node_t* vertex_node = get_vertex(graph, name);
    vertex_t* vertex = vertex_node->vertex;
    for (u_int32 i = 0; i < vertex->edges_size; i++){
        vertex_node_t* adj_vertex_node = get_vertex(graph, vertex->edges[i]);

        if (vertex_node->color == adj_vertex_node->color){
            char* name = (char*) get_hash(rep, vertex_node->vertex->name);
            increment_cost(name, vertex_node->vertex->name, rep, cost, conflict_vt);
            name = (char*) get_hash(rep, adj_vertex_node->vertex->name);
            increment_cost(name, adj_vertex_node->vertex->name, rep, cost, conflict_vt);
        }
    }
}

char** conflict_vertices(Graph* graph, int* size){
    hash_iterator_t* iter = graph->begin;
    char** conflict_vt = (char**) malloc(sizeof(char*) * graph->iterator_size);
    int k = 0;
    hash_table_t* rep = create_hash_table(graph->iterator_size, NULL);
    while (iter != NULL){
        vertex_node_t* vertex_node = (vertex_node_t*) iter->hash_node->value;
        vertex_t* vertex = vertex_node->vertex;
        char* name = (char*) get_hash(rep, vertex->name);
        if (name == NULL)
            count_cost(graph, vertex->name, rep, &k, conflict_vt);
        iter = iter->next;
    }
    free_hash_table(rep);
    *size = k;
    return conflict_vt;
}

int conflict_cost(Graph* graph, char** conflict_vertices, int size){
    int cost = 0;
    hash_table_t* rep = create_hash_table(graph->iterator_size, NULL);
    for (int i = 0; i < size; i++){
        count_cost(graph, conflict_vertices[i], rep, &cost, NULL);
    }
    free_hash_table(rep);
    return cost;
}

Graph* generate_candidates(Graph* graph, char** conflict_vt,
        int conflict_c, tabu_t** tabu_list, int* tabu_pos, 
        int best_cost){

    Graph* candidate = copy_graph(graph);

    for (int i = 0; i < conflict_c; i++){
        char* vertex_name = conflict_vt[i];
        vertex_node_t* vertex_node = get_vertex(candidate, vertex_name);
        int new_color = rand() % best_cost;
        int is_tabu = 0;
        for (int j = 0; j < *tabu_pos; j++){
            if (strcmp(tabu_list[j]->vertex_name, 
                        vertex_node->vertex->name) == 0 
                    && tabu_list[j]->color == new_color){
                
                is_tabu = 1;
            }
        }
        if (!is_tabu){
            vertex_node->color = new_color;
            tabu_list[*tabu_pos] = (tabu_t*) malloc(sizeof(tabu_t));
            vertex_node_t* tmp_vet = get_vertex(graph, vertex_node->vertex->name);
            tabu_list[*tabu_pos]->vertex_name = tmp_vet->vertex->name;
            tabu_list[*tabu_pos]->color = vertex_node->color;
            *tabu_pos += 1;
        }
    }

    return candidate;
}

Graph* get_best_candidate(Graph** candidates, int candidates_len, char** conflict_vt, 
        int conflict_len, int* best_cost){
    Graph* best = candidates[0];
    int conflict_best = conflict_cost(candidates[0], conflict_vt, conflict_len);
    for (int i = 1; i < candidates_len; i++){
        int conflict_c = conflict_cost(candidates[i], conflict_vt, conflict_len);
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

void update_tabu_list(tabu_t** tabu_list, int* tabu_pos, int tabu_len){
    int j = 0;
    for (int i = 0; i < *tabu_pos; i++){
        if (i >= *tabu_pos - tabu_len){
            tabu_list[j++] = tabu_list[i];
        }
        else free(tabu_list[i]);
    }
    *tabu_pos = tabu_len;
}

Graph* tabu_search(Graph* graph, int tabu_len, int candidates_len){
    Graph* best = initial_solution(graph);
    int best_cost = cost(best);

    srand(time(NULL));

    printf("Initial: %d\n", best_cost);

    int new_best_cost = best_cost - 1;
    Graph* s = reduce_color(best, new_best_cost);

    int conflict_c;
    char** conflict_vt = conflict_vertices(s, &conflict_c);

    int tabu_pos = 0;
    tabu_t** tabu_list = (tabu_t**) malloc(sizeof(tabu_t*) * candidates_len 
            * graph->iterator_size);

    int iter = 0;

    while (iter < MAX_ITER){
        Graph** candidates = (Graph**) malloc(sizeof(Graph*) * candidates_len);
        for (int i = 0; i < candidates_len; i++){
            candidates[i] = generate_candidates(s, conflict_vt, conflict_c,
                    tabu_list, &tabu_pos, new_best_cost);
        }

        int conflict_cand_c;
        Graph* best_candidate = get_best_candidate(candidates, candidates_len, 
                conflict_vt, conflict_c, &conflict_cand_c);

        if (conflict_c > conflict_cand_c){
            free_graph(s);
            free(conflict_vt);
            s = best_candidate;
            conflict_vt = conflict_vertices(s, &conflict_c);
        }
        else free_graph(best_candidate);

        if (conflict_c == 0){
            best_cost = new_best_cost;
            printf("Improvement: %d\n",best_cost);
            free_graph(best);
            best = s;

            new_best_cost = best_cost - 1;
            s = reduce_color(best, new_best_cost);
            free(conflict_vt);
            conflict_vt = conflict_vertices(s, &conflict_c);
        }

        if (tabu_pos > tabu_len)
            update_tabu_list(tabu_list, &tabu_pos, tabu_len);

        free(candidates);
        iter++;
    }

    free(conflict_vt);
    free(tabu_list);

    printf("Final: %d\n",best_cost);

    return best;
}
