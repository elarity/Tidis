/*
 * @desc : init the hashtable entry
 */
ht_st * init_ht() {
  ht_st * ht_entry;
  ht_entry = ( ht_st * )malloc( sizeof( ht_st ) ); 
  ht_entry->size = HT_SIZE;
  ht_entry->used = 0;
  //ht_entry->entry = ( ht_node_st * )calloc( ht_entry->size, sizeof( ht_node_st * ) );
  ht_entry->entry = calloc( ht_entry->size, sizeof( ht_node_st * ) );
  return ht_entry;
}

/*
 * @desc : init the hashtable entry
 */
ht_node_st * allocate_ht_node( const char * key, const char * value ) {
  ht_node_st * ht_node; 
  ht_node = ( ht_node_st * )malloc( sizeof( ht_node_st ) );
  ht_node->key   = strdup( key );
  ht_node->value = strdup( value );
  ht_node->next  = NULL;
  return ht_node; 
}

/*
 * @desc : destroy a hashtable
 */
void destroy_ht( ht_st * ht_entry ) {
  // 1.destroy ht_entry->entry 
  for ( int i = 0; i < ht_entry->size; i++ ) {
    ht_node_st * ht_node = ht_entry->entry[ i ];
    if ( NULL != ht_node ) {
      printf( "destroy node | bingo : %d\n", i );
      destroy_ht_node( ht_node );
    }
    else {
      printf( "destroy node | null\n" );
    }
  }
  // 2.destroy ht_entry
  free( ht_entry->entry );
  free( ht_entry );
}

/*
 * @desc : destroy a hashtable node
 */
void destroy_ht_node( ht_node_st * ht_node ) {
  ht_node_st * temp_ht_node;
  while ( ht_node ) {
    temp_ht_node = ht_node; 
    ht_node = ht_node->next;
    free( temp_ht_node->key ); 
    free( temp_ht_node->value ); 
    free( temp_ht_node );
  }
}

/*
 * @desc : add a new node to hash-table
 */
int add_ht_node( ht_st * ht_entry, const char * key, const char * value ) {
  ht_node_st * new_ht_node;
  ht_node_st * origin_ht_node;
  int          hash_index;
  hash_index = hash( key, strlen( key ), 0 ) % HT_SIZE;
  // if hash-collision
  origin_ht_node = ht_entry->entry[ hash_index ];
  if ( origin_ht_node ) {
    // zip method
    printf( "当前位置有节点\n" );
    while ( origin_ht_node ) {
      if ( NULL == origin_ht_node->next ) {
        new_ht_node = allocate_ht_node( key, value ); 
        origin_ht_node->next = new_ht_node;
        break;
      }
      else {
        origin_ht_node = origin_ht_node->next;
      }
    }
  }
  else {
    printf( "当前位置没有节点，第一次\n" );
    new_ht_node = allocate_ht_node( key, value ); 
    ht_entry->entry[ hash_index ] = new_ht_node;
  }
  return hash_index;
}

/*
 * @desc : find a key from hash-table
 */
ht_node_st * get_ht_node( ht_st * ht_entry, const char * key ) {
  int hash_index;
  ht_node_st * ht_node;
  hash_index = hash( key, strlen( key ), 0 ) % HT_SIZE;
  ht_node    = ht_entry->entry[ hash_index ]; 
  // exists hashtable_node @ hash_index
  if ( ht_node ) {
    while ( ht_node ) {
      printf( "get_ht_node\n" );
      if ( 0 == strcmp( key, ht_node->key ) ) {
        break;
      } 
      else {
        ht_node = ht_node->next;
      }
    } 
    return ht_node;
  }
  // if not exists @ hash_index, return NULL
  else {
    return NULL;
  }
}

/*
 * @desc : look up a key from a zip...
 */
ht_node_st * lookup( ht_node_st * ht_node, const char * key ) {
  if ( NULL == ht_node ) {
    return NULL;
  }
  while ( ht_node ) {
    if ( 0 == strcmp( key, ht_node->key ) ) {
      break;
    } 
    else {
      ht_node = ht_node->next;
    }
  }
  return ht_node; 
}

/*
 * @desc : delete a key from hashtable
 */
int delete_ht_node( ht_st * ht_entry, const char * key ) {
  int          hash_index;
  int          ret_index = 0;
  int          counter   = 0;
  ht_node_st * entry_ht_node;
  ht_node_st * followup_ht_node;
  hash_index   = hash( key, strlen( key ), 0 ) % HT_SIZE;

  entry_ht_node = ht_entry->entry[ hash_index ];
  // if there node @ temporary index
  if ( NULL == entry_ht_node ) {
    ret_index = 0;
  }
  // exists a node @ index
  else {
    ht_node_st * temp_node;
    temp_node = entry_ht_node;
    // while寻找到这个key
    while ( entry_ht_node ) {
      if ( 0 == strcmp( entry_ht_node->key, key ) ) {
        // first node bingo
        if ( 0 == counter ) {
          ht_entry->entry[ hash_index ] = NULL; 
        } 
        // follow-up node bingo
        else {
          if ( NULL == entry_ht_node->next ) {
            temp_node->next = NULL;
          }
          else {
            //printf( "%s %s %s\n", temp_node->key, entry_ht_node->key, entry_ht_node->next->key );
            //printf( "%p %p\n", temp_node->next, entry_ht_node->next );
            temp_node->next = entry_ht_node->next;
          } 
        }
        free( entry_ht_node->key );
        free( entry_ht_node->value );
        free( entry_ht_node );
        break;
      }
      else {
        temp_node     = entry_ht_node;
        entry_ht_node = entry_ht_node->next;
        counter++;
      }
    }
  }

  return ret_index;
}

/*
 * @desc : hash function based on murmurhash3
 *         copy from https://github.com/jwerle/murmurhash.c/blob/master/murmurhash.c 
 */
uint32_t hash ( const char *key, uint32_t len, uint32_t seed ) {
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m  = 5;
  uint32_t n  = 0xe6546b64;
  uint32_t h  = 0;
  uint32_t k  = 0;
  uint8_t *d  = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail    = NULL; // tail - last 8 bytes
  int i  = 0;
  int l  = len / 4; // chunk length
  h      = seed;
  chunks = (const uint32_t *) (d + l * 4); // body
  tail   = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'
  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
    k = chunks[i];
    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;
    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }
  k = 0;
  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
    case 2: k ^= (tail[1] << 8);
    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }
  h ^= len;
  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);
  return h;
}
