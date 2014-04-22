cc=gcc
flags=-g -Wall -Wextra -std=c11
defines=#-DPRINT_COSTS

all: sequential parallel

sequential:
	$(cc) $(flags) src/*.c src/sequential/*.c -o color_seq.out $(defines)

parallel:
	$(cc) $(flags) src/*.c src/parallel/*.c -o color_par.out -fopenmp

checker:
	make -C ./src/checker

run:
	./testall.sh 0

run-parallel:
	./testall.sh 1

clean:
	rm -rf *.out
