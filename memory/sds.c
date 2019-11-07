#include <stdlib.h>
#include "sds.h"
/*
 * @desc : init an empty sds struct
 */
sds_struct * sds_init( char * buf ) {
}
/*
 * @desc : empty sds buf
 */
void sds_empty( sds_struct * sds_struct_pointer ) {
  free( sds_struct_pointer->buf );
  sds_struct_pointer->length = 0;
}

void sds_strcat( char * dest, char * source ) {
}

void sds_strcpy( char * dest, char * source ) {
}

size_t sds_get_length( sds_struct * sds_struct_pointer ) {
  return sds_struct_pointer->length;
}

size_t sds_get_free( sds_struct * sds_struct_pointer ) {
  return sds_struct_pointer->free;
}
