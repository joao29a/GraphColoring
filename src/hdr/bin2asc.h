#ifndef BIN2ASC_H
#define BIN2ASC_H

#include <stdio.h>

/* If you change MAX_NR_VERTICES, change MAX_NR_VERTICESdiv8 to be 
the 1/8th of it */

#define MAX_NR_VERTICES		20000
#define MAX_NR_VERTICESdiv8	2500

int Nr_vert, Nr_edges;
char Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];

#define MAX_PREAMBLE 10000
static char Preamble[MAX_PREAMBLE];

void write_graph_DIMACS_ascii(char*, void*, void (*)(void*,int,int));

#endif
