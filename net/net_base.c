/*
 * @desc   : accept tcp connect
 * @param  : listen socket fd
 * @return : int fd
 */
int net_accept_tcp( int listen_socket_fd ) {
  int client_socket_fd; 
  struct sockaddr_in client_socket_struct;
  socklen_t client_socket_struct_length;
  client_socket_struct_length = sizeof( client_socket_struct );
  client_socket_fd = accept( listen_socket_fd, ( struct sockaddr * )&client_socket_struct, &client_socket_struct_length ); 
  return client_socket_fd;
}

/*
 * @desc : read data from tcp buffer
 */
void net_read_buf() {
}

/*
 * @desc : write data to tcp buffer
 */
void net_write_buf() {
}
