#include "hdr/graph.h"
#include "hdr/bin2asc.h"
#include "hdr/heap.h"
#include "hdr/tabu_search.h"

int main(int argc, char** argv){
    if (argc > 3){
        Graph *graph, *best;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);
        best = tabu_search(graph, atoi(argv[2]), atoi(argv[3]));
    }
    else printf("Missing arguments.\n");
    return 0;
}
