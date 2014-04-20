#ifndef TABU_H
#define TABU_H

#include <time.h>
#include <omp.h>
#include "../../hdr/heap.h"
#include "../../hdr/graph.h"
#include "../../hdr/hash.h"
#include "../../hdr/vertex.h"

typedef struct tabu_st{
    char* vertex_name;
    int color;
}tabu_t;

Graph* tabu_search(Graph*, int, int, int, int);

#endif
