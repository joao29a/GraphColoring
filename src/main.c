#include "hdr/graph.h"
#include "hdr/bin2asc.h"
#include "hdr/heap.h"
#include "hdr/tabu_search.h"

int main(int argc, char** argv){
    if (argc > 3){
        int tabu_size = atoi(argv[2]);
        int candidates_size = atoi(argv[3]);
        if (tabu_size <= 0 || candidates_size <= 0){
            printf("Candidates and tabu size must be higher than 0\n");
            return 1;
        }
        Graph *graph, *best;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);
        best = tabu_search(graph, atoi(argv[2]), atoi(argv[3]));
    }
    else printf("Insert file name, tabu size and candidates size.\n");
    return 0;
}
