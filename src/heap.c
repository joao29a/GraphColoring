#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

static void heapify(void** arr, size_t len, int i, int (*func)(void*,void*)){
    size_t largest;
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

void* heap_find(void** arr){
    return arr[0];
}

void heap_push(void** arr, void* elem, size_t* len, int (*func)(void*,void*)){
    if (*len == 0){
        arr = malloc(sizeof(void*));
        arr[0] = elem;
        *len += 1;
        build_heap(arr, *len, func);
    }
    else{
        void** temp = realloc(arr, sizeof(void*) * *len + 1);

        if (temp != NULL){
            arr = temp;
            arr[*len] = elem;
            build_heap(arr, *len + 1, func);
            *len += 1;
        }
    }
}

void* heap_pop(void** arr, size_t* len, int (*func)(void*,void*)){
    if (arr == NULL || *len == 0) return NULL;

    void* root = malloc(sizeof(void*));
    memcpy(root, heap_find(arr), sizeof(void*));

    void* dump = arr[0]; 
    arr[0] = arr[*len - 1];
    free(dump);

    void** temp = realloc(arr, sizeof(void*) * *len - 1);
    if (temp != NULL){
        arr = temp;
        build_heap(arr, *len - 1, func);
        *len -= 1;
    }
    else return NULL;

    return root;
}
