#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "main.h"
#include "function.c"

int main( int argc, char * argv[] ) {
  // initial server config
  server_config_struct * server_config;
  server_config = parse_config_file();
  if ( NULL == server_config ) {
    printf( "parse config file error.\n" );
    return -1;
  }
  // daemonize the program...like Nginx OR Redis
  if ( 0 == strcmp( "yes", server_config->daemonize ) ) { 
    daemonize();
  }

  // event loop flow:begin
  ev_loop_struct * ev = init_ev( 1024 );

  // create a single process & thread tcp server
  server_struct * server = init_server( server_config, ev ); 

  // begin event-loop
  ev_main( ev );
  return 0;
}

/*
 * @desc   : parse config file to server config structure 
 * @return : pointer of server_config_struct
 */
server_config_struct * parse_config_file() {
  server_config_struct * server_config = ( server_config_struct * )malloc( sizeof( server_config_struct ) );
  /*
  config_item_struct config_item_list[] = {
    { "host", "" },
    { "port", "" },
    { "event", "" },
    { "daemonize", "" },
  };
   */
  int  buffer_size = 64;
  int  config_item_list_size;
  char config_item[ buffer_size ];
  char * key;
  char * value;
  char * _config_item;
  char * delimeter = "=";
  FILE * config_file;
  if ( ( config_file = fopen( "./conf/tidis.conf", "r" ) ) < 0 ) {
    return NULL;
  } 
  //config_item_list_size = sizeof( config_item_list ) / sizeof( config_item_struct );
  while ( fgets( config_item, buffer_size, config_file ) ) {
    _config_item = config_item;
    // NOTICE : strtok is deprecated, just use strsep( Thread Satety! )
    key   = strsep( &_config_item, delimeter ); 
    value = _config_item;
    trim( key );
    trim( value );
    if ( 0 == strcmp( key, "host" ) ) {
      strcpy( server_config->host, value ); 
    } else if ( 0 == strcmp( key, "port" ) ) {
      strcpy( server_config->port, value ); 
    } else if ( 0 == strcmp( key, "event" ) ) {
      strcpy( server_config->event, value ); 
    } else if ( 0 == strcmp( key, "daemonize" ) ) {
      strcpy( server_config->daemonize, value ); 
    }
    //free( key );
    //free( value );
    /*
    for ( int i = 0; i < config_item_list_size; i++ ) {
      if ( 0 == strcmp( key, config_item_list[ i ].key ) ) {
        strcpy( config_item_list[ i ].value, value );
      } 
    }
     */
  }
  return server_config;
}

/*
 * @desc  : creata a tcp server( single process & thread )
 * @param : server_config_struct point
 */
server_struct * init_server( server_config_struct * server_config, ev_loop_struct * ev ) {
  server_struct * server = ( server_struct * )malloc( sizeof( server_struct ) );
  server->config = server_config;
  struct sockaddr_in listen_socket_struct;
  int    listen_socket_fd;
  int    lis_sock_opt_addr_reuse = 1;
  struct sockaddr_in client_socket_struct;
  int    client_socket_fd;
  socklen_t client_socket_struct_length;
  int    common_ret;
  // bzero:Not standard ANSI,deprecated! Sugguest memset~
  //bzero( &listen_socket_struct, sizeof( listen_socket_struct ) );
  memset( &listen_socket_struct, 0, sizeof( listen_socket_struct ) );
  listen_socket_struct.sin_family = AF_INET;
  // remember atoi
  listen_socket_struct.sin_port   = htons( atoi( server_config->port ) );
  inet_pton( AF_INET, server_config->host, &listen_socket_struct.sin_addr );
  // create listen socket.NOTE:if you do not know what exactly the last parameter means,just keep it 0
  listen_socket_fd = socket( PF_INET, SOCK_STREAM, 0 ); 
  set_nonblock( listen_socket_fd );
  if ( listen_socket_fd < 0 ) {
    printf( "create socket error." );
    exit( -1 ); 
  }
  // fcntl
  setsockopt( listen_socket_fd, SOL_SOCKET, SO_REUSEADDR, 
              &lis_sock_opt_addr_reuse, sizeof( lis_sock_opt_addr_reuse ) );
  // convert sockaddr_in -> sockaddr_stroage,I think sockaddr_stroage would be better than sockaddr
  common_ret = bind( listen_socket_fd, ( struct sockaddr * )&listen_socket_struct, sizeof( listen_socket_struct ) ); 
  if ( common_ret < 0 ) {
    printf( "bind socket error." );
    exit( -1 ); 
  }
  // make the socket in listening.Second parameter is backlog
  common_ret = listen( listen_socket_fd, 10 );
  if ( common_ret < 0 ) {
    printf( "listen socket error." );
    exit( -1 ); 
  }
  server->listen_socket_fd = listen_socket_fd;

  // create an event-struct for listen_socket_fd
  ev_create_file_event( ev, listen_socket_fd, accept_tcp_connect_processor, EV_READABLE ); 

  return server;
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

/*
 * @desc : unix standard daemon function.
 *         1.Some guy may like fork twice( a little bird says [fork twice] to avoid SVR4 bugs,in some case,
             fork once can not make the program separate from the CLI-terminal ),BUT I do it only once,you
             may add the second fork to the code
           2.The Linux standard libray supply a API in the unistd.h header file:daemon( int, int )
 */
bool daemonize() {
  pid_t pid; 
  pid = fork();
  // fork error.
  if ( pid < 0 ) {
    return false;
  }
  // exit father process 
  if ( pid > 0 ) {
    // notice:NOT return,BUT exit!
    exit( 0 );
  } 
  // from here,let rock the child process flow
  umask( 0 );
  pid_t sid;
  sid = setsid(); 
  if ( sid < 0 ) {
    return false;
  } 
  // change work directory
  /*
   chdir();
   */
  // close STDIN STDOUT STDERR
  close( STDIN_FILENO ); 
  close( STDOUT_FILENO ); 
  close( STDERR_FILENO ); 
  // redirect STDIN STDOUT STDERR to /dev/null OR somewhere else
  // /dev/null means : all the data-flow to /dev/null will directly missing
  // little tip:open three "/dev/null" right after closing three STD-FILENO means the program will reuse file-no 0 1 2,bcz open sys-call will return the minimum number of the UN-USED file-no
  // 0=STDIN_FILENO 1=STDOUT_FILENO 2=STDERR_FILENO
  // dup/dup2 function could do the same work also
  open( "/dev/null", O_RDONLY );
  open( "/dev/null", O_RDWR );
  open( "/dev/null", O_RDWR );
  return true; 
}
