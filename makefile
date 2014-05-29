cc=gcc
flags=-g -Wall -Wextra -std=c11
inc_mpi=-I/usr/include/mpi
ld=-lmpi -lpthread -lmpc
defines=#-DPRINT_COSTS

all: sequential parallel mpi

sequential:
	$(cc) $(flags) src/*.c src/sequential/*.c -o color_seq.out $(defines)

parallel:
	$(cc) $(flags) src/*.c src/parallel/*.c -o color_par.out -fopenmp

mpi:
	$(cc) $(flags) src/*.c src/mpi/*.c -o color_mpi.out $(inc_mpi) $(ld)

checker:
	make -C ./src/checker

run:
	./testall.sh 0

run-parallel:
	./testall.sh 1

clean:
	rm -rf *.out
