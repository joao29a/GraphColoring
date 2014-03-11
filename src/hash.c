#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "hdr/hash.h"

//djb2 algorithm
int hash_function(hash_table_t* table, char* key){
    u_int32 hash = 5381;
    unsigned int i;
    for (i = 0; i < strlen(key); i++){    
        hash = ((hash << 5) + hash) + key[i];
    }
    return hash % table->size;
}

hash_table_t* create_hash_table(int size){
    hash_table_t* table = NULL;

    table = (hash_table_t*) malloc(sizeof(hash_table_t));
    table->size = size;
    table->begin = NULL;
    table->end = NULL;
    table->node = (hash_node_t**) malloc(sizeof(hash_node_t*)*size);

    int i;
    for (i = 0; i < size; i++){
        table->node[i] = NULL;
    }

    return table;
}

hash_node_t* create_pair(char* key, void* value){
    hash_node_t* node = (hash_node_t*) malloc(sizeof(hash_node_t));
    node->key = (char*) malloc(sizeof(char) * strlen(key));
    strcpy(node->key, key);
    node->value = value;
    node->next = NULL;
    return node;
}

void set_hash_iterator(hash_table_t* table, hash_node_t* node){
    if (table->begin == NULL){
        table->begin = (hash_iterator_t*) malloc(sizeof(hash_iterator_t));
        table->begin->hash_node = node;
        table->begin->next = NULL;
        table->end = table->begin;
    } else {
        table->end->next = (hash_iterator_t*) malloc(sizeof(hash_iterator_t));
        table->end = table->end->next;
        table->end->hash_node = node;
        table->end->next = NULL;
    }
}

void set_hash(hash_table_t* table, char* key, void* value){
    int index = hash_function(table, key);

    hash_node_t* node = table->node[index];

    if (node == NULL){ 
        table->node[index] = create_pair(key, value);
        set_hash_iterator(table, table->node[index]);
    }
    else{
        while (node->next != NULL && strcmp(node->key, key) != 0)
            node = node->next;
        if (strcmp(node->key, key) == 0) node->value = value;
        else if (node->next == NULL){
            node->next = create_pair(key, value);
            set_hash_iterator(table, node->next);
        }
    }
}

void* get_hash(hash_table_t* table, char* key){
    int index = hash_function(table, key);

    hash_node_t* node = table->node[index];

    if (node == NULL) return NULL;
    else{
        while (node->next != NULL && strcmp(node->key, key) != 0)
            node = node->next;
        if (strcmp(node->key, key) == 0) return node->value;
    }

    return NULL;
}

void free_hash_table(hash_table_t* table, void (*free_value)(void*)){
    hash_iterator_t* i = table->begin;
    while (i != NULL){    
        free(i->hash_node->key);
        free_value(i->hash_node->value);
        free(i->hash_node);
        hash_iterator_t* dump = i;
        i = i->next;
        free(dump);
    }
    free(table->node);
    free(table);
}
