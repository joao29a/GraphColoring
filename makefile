all:
	clang -g -Wall -Wextra -Werror src/*.c -o out -I./src/hdr/ -std=c11
