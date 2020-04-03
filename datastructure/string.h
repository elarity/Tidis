typedef struct sds_struct {
  size_t free;
  size_t length;
  char   buf[]; 
} sds_struct;
sds_struct * sds_init( char * );
void sds_empty( sds_struct * );
void sds_strcat( char *, char * );
void sds_strcpy( char *, char * );
size_t sds_get_length( sds_struct * );
size_t sds_get_free( sds_struct * );
