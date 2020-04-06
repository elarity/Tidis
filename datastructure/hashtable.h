#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "object.h"

#define HT_SIZE 1024

// hashtable node struct
typedef struct hashtable_node {
    char * key;
    void * value;   // 可能会指向任何类型，string、hashtable、list、set、zset等
    struct hashtable_node * next;  //为拉链法解决hash collision
} ht_node_st;

// hashtable entry
typedef struct hashtable_entry {
    size_t size;
    size_t used;
    ht_node_st ** entry;
} ht_st;

// function prototype
// init a hashtable
ht_st * init_ht();
// create a new hashtable node
ht_node_st * allocate_ht_node( char *, void * );
// destroy a hashtable
void destroy_ht( ht_st * );
// destroy a hashtable node
void destroy_ht_node( ht_node_st * );
// get a key
ht_node_st * get_ht_node( ht_st *, tidis_object_struct * tidis_object_key );
// insert a key
int add_ht_node( ht_st *, tidis_object_struct *, void * );
// delete a key
int delete_ht_node( ht_st *, const char * );
// hash algorithm based on murmurhash3
uint32_t hash( const char *, uint32_t, uint32_t );
