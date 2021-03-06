#include "../hdr/graph.h"
#include "../hdr/bin2asc.h"
#include "../hdr/heap.h"
#include "hdr/tabu_search.h"

int main(int argc, char** argv){
    if (argc > 4){
        int tabu_size = atoi(argv[2]);
        int candidates_size = atoi(argv[3]);
        int iterations = atoi(argv[4]);
        if (tabu_size <= 0 || candidates_size <= 0 || iterations <= 0){
            printf("Candidates, tabu and iterations size must be higher than 0\n");
            return 1;
        }
        Graph *graph, *best;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);
        best = tabu_search(graph, tabu_size, candidates_size, iterations);
#ifndef PRINT_COSTS
        print_graph_color(best);
#endif
    }
    else printf("Insert file name, tabu size, candidates size and iterations.\n");
    return 0;
}
