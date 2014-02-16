#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void** create_heap(size_t);
static void heapify(void**, size_t, int, int(*)(void*,void*));
static void heap_increase_key(void**, int, int (*)(void*,void*));
void build_heap(void**, size_t, int(*)(void*,void*));
void* heap_find(void**);
void heap_push(void**, void*, size_t*, int(*)(void*,void*));
void* heap_pop(void**, size_t*, int(*)(void*,void*));

#endif
