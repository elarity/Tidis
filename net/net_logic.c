/*
 * This file is logic for net_base.c
 * If net_base.c is "model", this file(net_logic.c) is "logic"
 */
#include <stdbool.h>
#include "../main.h"
#include "net_base.c"

// set a fd to non-blocking
void set_nonblock( int fd ) {
  int origin_fd_option = fcntl( fd, F_GETFL ); 
  int new_fd_option    = origin_fd_option | O_NONBLOCK; 
  fcntl( fd, F_SETFL, new_fd_option );
}

/*
 * @desc  : processor for processing TCP accept action
 * @param : ev_loop, event loop 
 * @param : fd, listen_socket_fd
 */
void accept_tcp_connect_processor( ev_loop_struct * ev_loop, int listen_socket_fd ) {
  int client_socket_fd; 
  client_socket_fd = accept_tcp_connection( listen_socket_fd ); 
  set_nonblock( client_socket_fd );
  ev_create_file_event( ev_loop, client_socket_fd, read_from_client, EV_READABLE );
}
