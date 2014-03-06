all: compile

compile:
	clang -g -Wall -Wextra src/*.c -o color.out -std=c11 -fopenmp
