/*
 * @desc   : accept tcp connect
 * @param  : listen socket fd
 * @return : int fd
 */
int accept_tcp_connection( int listen_socket_fd ) {
  int client_socket_fd; 
  struct sockaddr_in client_socket_struct;
  socklen_t client_socket_struct_length;
  client_socket_struct_length = sizeof( client_socket_struct );
  client_socket_fd = accept( listen_socket_fd, ( struct sockaddr * )&client_socket_struct, &client_socket_struct_length ); 
  //set_nonblock( client_socket_fd );
  //ev_create_file_event( ev_loop, client_socket_fd, read_from_client, EV_READABLE );
  return client_socket_fd;
}
