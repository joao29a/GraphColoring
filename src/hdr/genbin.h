#ifndef GENBIN_H
#define GENBIN_H

#include <stdio.h>

/* If you change MAX_NR_VERTICES, change MAX_NR_VERTICESdiv8 to be 
the 1/8th of it */

#define MAX_NR_VERTICES		20000
#define MAX_NR_VERTICESdiv8	2500

#define BOOL	char

int Nr_vert, Nr_edges;
BOOL Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];

#define MAX_PREAMBLE 10000
static char Preamble[MAX_PREAMBLE];

#endif
