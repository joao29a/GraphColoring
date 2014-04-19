#ifndef TABU_H
#define TABU_H

#include "heap.h"
#include "graph.h"
#include "hash.h"
#include "vertex.h"
#include <time.h>

typedef struct tabu_st{
    char* vertex_name;
    int color;
}tabu_t;

Graph* tabu_search(Graph*, int, int, int);

#endif
