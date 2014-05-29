#include "../hdr/graph.h"
#include "../hdr/bin2asc.h"
#include "../hdr/heap.h"
#include "hdr/tabu_search.h"
#include <mpi.h>
#include <pthread.h>
#include <limits.h>

typedef struct pthread_args{
    int world_size;
    int* colors;
}pthread_args_t;

void receive_message(pthread_args_t* args){
    int received_messages = 0;
    while (received_messages < args->world_size - 1){
        int color;
        MPI_Recv(&color, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        args->colors[received_messages++] = color;
    }
}

int main(int argc, char** argv){
    if (argc > 4){
        int tabu_size = atoi(argv[2]);
        int candidates_size = atoi(argv[3]);
        int iterations = atoi(argv[4]);
        if (tabu_size <= 0 || candidates_size <= 0 || iterations <= 0){
            printf("Candidates, tabu and iterations size must be higher than 0\n");
            return 1;
        }
        Graph *graph;
        graph = create_graph();
        read_file_on_graph(graph, argv[1]);

        MPI_Init(NULL, NULL);
        int world_size, world_rank;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        pthread_t* thread;
        pthread_args_t args;

        if (world_rank == 0){
            thread = (pthread_t*) malloc(sizeof(pthread_t));
            args.world_size = world_size;
            args.colors = (int*) malloc(sizeof(int)*world_size);
            pthread_create(thread, NULL, (void*(*)(void*)) &receive_message, &args);
        }

        iterations /= world_size;
        candidates_size /= world_size;
        int colors;
        srand(time(NULL) / (world_rank + 1)); // this is tricky
        tabu_search(graph, tabu_size, candidates_size, iterations, &colors);

        if (world_rank == 0){
            printf("0 %d colors\n", colors);
            pthread_join(*thread, NULL);
            free(thread);
            args.colors[world_size - 1] = colors;
            int least_color = INT_MAX;
            for (int i = 0; i < world_size; i++){
               if (args.colors[i] < least_color)
                   least_color = args.colors[i];
            }
            free(args.colors);
            printf("Colors used: %d\n", least_color);
        }
        else {
            printf("%d sending %d\n", world_rank, colors);
            MPI_Send(&colors, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        MPI_Finalize();
        
    }
    else printf("Insert file name, tabu size, candidates size and iterations.\n");
    return 0;
}
