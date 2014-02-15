#ifndef HASH_H
#define HASH_H

typedef unsigned long int u_int32;

struct hash_node{
    char* key;
    void* value;
    struct hash_node* next;
};

typedef struct hash_node hash_node_t;

struct hash_table{
    int size;
    hash_node_t** node;
};

typedef struct hash_table hash_table_t;

static int hash_function(hash_table_t*, char*);
static hash_node_t* create_pair(char*, void*);
void free_hash_table(hash_table_t* table, void (*)(void*));
hash_table_t* create_hash_table(int);
void set_hash(hash_table_t*, char*, void*);
void* get_hash(hash_table_t*, char*);

#endif
