#include "hdr/graph.h"
#include "hdr/bin2asc.h"
#include "hdr/heap.h"
#include "hdr/ant_system.h"

int main(int argc, char** argv){
    if (argc > 1){
        Graph *graph, *best;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);
        best = ant_system(graph);
    }
    else printf("Missing arguments.\n");
    return 0;
}
