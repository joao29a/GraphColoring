all: compile

compile:
	clang -g -Wall -Wextra src/*.c -o out -std=c11 -fopenmp
