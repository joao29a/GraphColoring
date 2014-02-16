#include "heap.h"

/* define the max size of the container */
void** create_heap(size_t size){
    return (void**) malloc(sizeof(void*) * size);
}

void heapify(void** arr, size_t len, int i, int (*func)(void*,void*)){
    int largest;
    size_t l = 2 * i;
    size_t r = 2 * i + 1;

    if (l < len && func(arr[l], arr[i])) largest = l;
    else largest = i;
    if (r < len && func(arr[r], arr[largest])) largest = r;

    if (largest != i){
        void* temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, len, largest, func);
    }
}

void build_heap(void** arr, size_t len, int (*func)(void*,void*)){
    int i;
    for (i = len / 2; i >= 0; i--){
        heapify(arr, len, i, func);
    }
}

void* heap_peek(void** arr){
    return arr[0];
}

void heap_key(void** arr, int i, int (*func)(void*,void*)){
    while (i > 0 && func(arr[i], arr[i/2])){
        void* temp = arr[i];
        arr[i] = arr[i/2];
        arr[i/2] = temp;
        i = i/2;
    }
}

void heap_push(void** arr, void* elem, size_t* len, int (*func)(void*,void*)){
    arr[*len] = elem;
    heap_key(arr, *len, func);
    *len += 1;
}

void* heap_pop(void** arr, size_t* len, int (*func)(void*,void*)){
    if (arr == NULL || *len == 0) return NULL;
    void* root = heap_peek(arr);
    arr[0] = arr[*len - 1];
    *len -= 1;
    heapify(arr, *len, 0, func);
    return root;
}
