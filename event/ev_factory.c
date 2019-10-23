#include "ev_factory.h"

// init the ev_loop_struct
ev_loop_struct * init_ev( int size ) {
  ev_loop_struct * ev = ( ev_loop_struct * )malloc( sizeof( ev_loop_struct ) );
  ev->event_size = size;
  if ( NULL == ev ) {
    printf( "can not locate enough 4 event-loop" );
    return NULL;
  }
  ev->file_events = ( ev_file_event_struct * )malloc( sizeof( ev_file_event_struct ) * size );
  if ( NULL == ev ) {
    printf( "can not locate enough 4 file event" );
    return NULL;
  }
  ev->fire_events = ( ev_fire_event_struct * )malloc( sizeof( ev_fire_event_struct ) * size );
  if ( NULL == ev ) {
    printf( "can not locate enough 4 fire event" );
    return NULL;
  }
  create_fd_entry( ev ); 
  // init every fd-event's mask
  for ( int i = 0; i < size; i++ ) {
    (ev->file_events[ i ]).event_type = EV_NONE;
  }
  ev->tv_out = NULL;
  ev->max_fd = -1;  // notice:0 1 2
  return ev;
}

/*
 * @desc  : create an file event
 * @param : ev_loop
 * @param : fd
 * @param : processor, event processor
 * @param : event_type
 */
void ev_create_file_event( ev_loop_struct * ev_loop, int fd, file_event_processor * processor, int event_type ) {
  // the memory for file event struct of fd is created when in init_ev function.
  // so, we directly take it out 
  ev_file_event_struct * file_event = &( ev_loop->file_events[ fd ] ); 
  file_event->read_processor  = processor; 
  file_event->write_processor = processor; 
  ev_add_event( ev_loop, fd, event_type );
  ev_loop->max_fd = fd;
}

/* 
 * @desc   : begin an event loop
 * @return : return activity fd
 */
int ev_process( ev_loop_struct * ev_loop ) {
  int activity_fd_num;
  // the server will lose in LOOP!
  for( ; ; ) {
    // block at SELECT/EPOLL/POLL system call
    activity_fd_num = poll_event( ev_loop ); 
    printf( "activity-fd-number : %d\n", activity_fd_num );
    
    typedef struct ev_fd_entry_struct {
      fd_set read_fds;
      fd_set _read_fds;
      fd_set write_fds;
      fd_set _write_fds;
    } ev_fd_entry_struct;
    if ( activity_fd_num > 0 ) {
      for ( int i = 0; i <= ev_loop->max_fd; i++ ) {
        ev_fd_entry_struct * fd_entry = ev_loop->fd_entry; 
        // take out the event for the fd from ev_loop->events
        ev_file_event_struct * file_event = &( ev_loop->file_events[ i ] ); 
        /*
        if ( FD_ISSET( i, &( fd_entry->_read_fds ) ) ) { 
          if ( 4 == i ) {
					  int client_socket_fd; 
					  struct sockaddr_in client_socket_struct;
					  socklen_t client_socket_struct_length;
					  client_socket_struct_length = sizeof( client_socket_struct );
					  client_socket_fd = accept( i, ( struct sockaddr * )&client_socket_struct, &client_socket_struct_length ); 
            FD_SET( client_socket_fd, &( fd_entry->read_fds ) );
            printf( "heheheheheheh\n" );
            ev_loop->max_fd = client_socket_fd;
          } 
          else {
            printf( "hhahahahhahaha\n" );
            size_t  recv_buf_length = 30;
            char    recv_buf[ recv_buf_length ];
            ssize_t recv_length; 
            recv_length = recv( i, recv_buf, recv_buf_length, 0 );
            printf( "%s\n", recv_buf );
            if ( -1 == recv_length ) {
              printf( "%s\n", strerror( errno ) );
              return -1;
            }
            send( i, "helloclient", 12, 0 ); 
            FD_CLR( i, &fd_entry->read_fds );
            close( i );
          }
        } else { printf( "not in read_fds\n" ); }
        */
      }
    }
  }
  return 0;
}

/*
 * @desc  : event main function
 */
void ev_main( ev_loop_struct * ev_loop ) {
  ev_process( ev_loop ); 
}
