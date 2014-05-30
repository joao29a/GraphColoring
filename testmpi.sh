#!/bin/bash

tabu=30
cadidates=10000
iterations=5000
for i in $(find ./input -type f); do
    for j in {1..10}; do
        mpiexec -hostfile hosts color_mpi.out $i $candidates $iterations
    done
done
