/*
 * This file is logic for net_base.c
 * If net_base.c is "model", this file(net_logic.c) is "logic"
 */
#include <stdbool.h>
#include "../main.h"
#include "net_base.c"

// set a fd to non-blocking
void net_set_nonblock( int fd ) {
  int origin_fd_option = fcntl( fd, F_GETFL ); 
  int new_fd_option    = origin_fd_option | O_NONBLOCK; 
  fcntl( fd, F_SETFL, new_fd_option );
}

/*
 * @desc  : processor for processing TCP accept action
 * @param : ev_loop, event loop 
 * @param : fd, listen_socket_fd
 */
void net_accept_tcp_connect_processor( ev_loop_struct * ev_loop, int listen_socket_fd ) {
  int client_socket_fd; 
  client_socket_fd = net_accept_tcp( listen_socket_fd ); 
  net_set_nonblock( client_socket_fd );
  ev_create_file_event( ev_loop, client_socket_fd, read_from_client, EV_READABLE );
}

void read_from_client( ev_loop_struct * ev_loop, int fd ) {
  //printf( "main.c | read_from_client \n" ); 
  size_t  recv_buf_length = 3000;
  char    recv_buf[ recv_buf_length ];
  ssize_t recv_length; 
  recv_length = recv( fd, recv_buf, recv_buf_length, 0 );
  printf( "%s", recv_buf );
  if ( -1 == recv_length ) {
    printf( "%s\n", strerror( errno ) );
    return;
  }
  //send( fd, "helloclient", 12, 0 ); 
}
