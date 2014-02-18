/* Dimacs graph format translator to and from a binary, more efficient
   format. Written by Tamas Badics (badics@rutcor.rutgers.edu), 
   using the technique of Marcus Peinado. */

/* Marcus Peinado
   Department of Computer Science
   Boston University
   */

#include <stdlib.h>
#include "bin2asc.h"

char masks[ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

int get_params()
    /* getting Nr_vert and Nr_edge from the preamble string, 
       containing Dimacs format "p ??? num num" */
{
    char c, *tmp;
    char * pp = Preamble;
    int stop = 0;
    tmp = (char *)calloc(100, sizeof(char));

    Nr_vert = Nr_edges = 0;

    while (!stop && (c = *pp++) != '\0'){
        switch (c)
        {
            case 'c':
                while ((c = *pp++) != '\n' && c != '\0');
                break;

            case 'p':
                sscanf(pp, "%s %d %d\n", tmp, &Nr_vert, &Nr_edges);
                stop = 1;
                break;

            default:
                break;
        }
    }

    free(tmp);

    if (Nr_vert == 0 || Nr_edges == 0)
        return 0;  /* error */
    else
        return 1;

}

/* ====================================================== */

BOOL get_edge(i, j)
    int i;
    int j;
{
    int byte, bit;
    char mask;

    bit  = 7-(j & 0x00000007);
    byte = j >> 3;

    mask = masks[bit];
    return( (Bitmap[i][byte] & mask)==mask );
}



void read_graph_DIMACS_bin(file)
    char *file;
{

    int i, length = 0;
    FILE *fp;

    if ( (fp=fopen(file,"r"))==NULL )
    { printf("ERROR: Cannot open infile\n"); exit(10); }

    if (!fscanf(fp, "%d\n", &length))
    { printf("ERROR: Corrupted preamble.\n"); exit(10); }

    if(length >= MAX_PREAMBLE)
    { printf("ERROR: Too long preamble.\n"); exit(10); }

    fread(Preamble, 1, length, fp);
    Preamble[length] = '\0';

    if (!get_params())
    { printf("ERROR: Corrupted preamble.\n"); exit(10); }

    for ( i = 0
            ; i < Nr_vert && fread(Bitmap[i], 1, (int)((i + 8)/8), fp)
            ; i++ );

    fclose(fp);
}


/* ============================================= */

/* my changes */
void write_graph_DIMACS_ascii(file, graph, callback)
    char* file;
    void* graph;
    void (*callback)(void*,int,int);
{
    read_graph_DIMACS_bin(file);

    int i,j;
    
    for ( i = 0; i<Nr_vert; i++ )
    {
        for ( j=0; j<=i; j++ )
            if ( get_edge(i,j) ) callback(graph, i+1, j+1);
    }
}
