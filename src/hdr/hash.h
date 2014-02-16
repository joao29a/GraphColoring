#ifndef HASH_H
#define HASH_H

typedef unsigned long int u_int32;

typedef struct hash_node{
    char* key;
    void* value;
    struct hash_node* next;
} hash_node_t;

typedef struct hash_iterator{
    hash_node_t* hash_node;
    struct hash_iterator *next;
} hash_iterator_t;

typedef struct hash_table{
    int size;
    hash_iterator_t *begin;
    hash_iterator_t *end;
    hash_node_t** node;
} hash_table_t;

void set_hash_iterator(hash_table_t*, hash_node_t*);
int hash_function(hash_table_t*, char*);
hash_node_t* create_pair(char*, void*);
void free_hash_table(hash_table_t* table, void (*)(void*));
hash_table_t* create_hash_table(int);
void set_hash(hash_table_t*, char*, void*);
void* get_hash(hash_table_t*, char*);

#endif
