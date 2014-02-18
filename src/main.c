#include "graph.h"
#include "bin2asc.h"
#include "heap.h"
#include "greed_coloring.h"

int main(int argc, char** argv){
    if (argc > 1){
        Graph* graph = create_graph(argv[1]);
        printf("total colors: %zu\n", greed_coloring(graph));
    }
    else printf("Missing arguments.\n");
    return 0;
}
