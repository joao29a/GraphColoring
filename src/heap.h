#ifndef HEAP_H
#define HEAP_H

static void heapify(void**, size_t, int, int(*)(void*,void*));
void build_heap(void**, size_t, int(*)(void*,void*));
void* heap_find(void**);
void heap_push(void**, void*, size_t*, int(*)(void*,void*));
void* heap_pop(void**, size_t*, int(*)(void*,void*));

#endif
