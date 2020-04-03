#ifndef __TIDIS_H
#define __TIDIS_H

#include "config.h"
// event loop
#include "event/ev_factory.h"
// data structure
#include "datastructure/string.h"
#include "datastructure/hashtable.h"

// server config struct
// I prefer char array other than char * in struct
// main.c
typedef struct server_config_struct {
  char host[ 32 ];
  char port[ 8 ];
  char event[ 8 ];
  char daemonize[ 4 ];
} server_config_struct;
typedef struct config_item {
  char key[ 16 ]; 
  char value[ 32 ]; 
} config_item_struct;
typedef struct db_struct {
} db_struct;
// client struct
typedef struct client_struct {
  int fd;
  struct sds_struct * io_buffer; // input buffer, data from client to server
} client_struct;
// server struct in main.c
typedef struct server_struct {
  server_config_struct * config;
  int listen_socket_fd; 
  // db link list 
  // client link list 
} server_struct;

// main.c : function prototype
server_config_struct * parse_config_file();
server_struct        * init_server( server_config_struct *, ev_loop_struct * );
void net_accept_tcp_connect_processor( ev_loop_struct *, int );
void read_from_client( ev_loop_struct *, int );
bool daemonize();
void net_set_nonblock( int );

// function.c
void ltrim( char * );
void rtrim( char * );
void trim( char * );

#endif
