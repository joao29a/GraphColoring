all:
	clang -g -Wall -Wextra src/*.c *.c -o out -I./src/hdr/ -std=c11
