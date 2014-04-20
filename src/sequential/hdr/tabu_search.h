#ifndef TABU_H
#define TABU_H

#include "../../hdr/heap.h"
#include "../../hdr/graph.h"
#include "../../hdr/hash.h"
#include "../../hdr/vertex.h"
#include <time.h>

typedef struct tabu_st{
    char* vertex_name;
    int color;
}tabu_t;

Graph* tabu_search(Graph*, int, int, int);

#endif
