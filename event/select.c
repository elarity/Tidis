#include <sys/select.h>

/*
 * @desc : fd for select API
 */
typedef struct ev_fd_entry_struct {
  fd_set read_fds;
  fd_set _read_fds;
  fd_set write_fds;
  fd_set _write_fds;
} ev_fd_entry_struct;

/*
 * @desc : create fd entry for ev_factory's ev_loop struct
 */
static void create_fd_entry( ev_loop_struct * ev_loop ) {
  ev_fd_entry_struct * fd_entry = ( ev_fd_entry_struct * )malloc( sizeof( ev_fd_entry_struct ) );
  // clear the fd_set to ZERO!
  FD_ZERO( &( fd_entry->read_fds ) );
  FD_ZERO( &( fd_entry->_read_fds ) );
  FD_ZERO( &( fd_entry->write_fds ) );
  FD_ZERO( &( fd_entry->_write_fds ) );
  ev_loop->fd_entry = fd_entry;
}

/*
 * @desc : add a fd to fd_set
 */
static void ev_add_event( ev_loop_struct * ev_loop, int fd, int event_type ) {
  ev_fd_entry_struct * fd_entry = ev_loop->fd_entry;
  // add to different fd_set via different event_type
  if ( EV_READABLE == event_type ) {
    // if you can'not remember whether -> first or & first
    // so song( -_- ) a little, just use () to help you 
    FD_SET( fd, &( fd_entry->read_fds ) ); 
  }
  if ( EV_WRITEABLE == event_type ) {
    FD_SET( fd, &( fd_entry->write_fds ) );
  } 
}

/*
 * @desc : delete a fd to fd_set
 */
static void ev_del_event( ev_loop_struct * ev_loop, int fd, int event_type ) {
  ev_fd_entry_struct * fd_entry = ev_loop->fd_entry;
  if ( EV_READABLE == event_type ) {
    FD_CLR( fd, &( fd_entry->read_fds ) ); 
  }
  if ( EV_WRITEABLE == event_type ) {
    FD_CLR( fd, &( fd_entry->write_fds ) );
  }
}

/* 
 * @desc   : begin an event loop
 * @return : return activity fd
 */
static int poll_event( ev_loop_struct * ev_loop ) {
  ev_fd_entry_struct * fd_entry = ev_loop->fd_entry;
  int activity_fd_num;
  int activity_fd_ret = 0;
  // 1. DO NOT use fd_entry->_read_fds = fd_entry->read_fds
  // 2. You should know the difference between memset & strcpy
  memcpy( &fd_entry->_read_fds,  &fd_entry->read_fds,  sizeof( fd_set ) );
  memcpy( &fd_entry->_write_fds, &fd_entry->write_fds, sizeof( fd_set ) );
  activity_fd_num = select( ev_loop->max_fd + 1, &fd_entry->_read_fds, NULL, NULL, NULL );
  // promise me, remember put the read-ready or write-ready fd to ev_loop->fire_events array... 
  if ( activity_fd_num > 0 ) {
    for ( int i = 0; i <= ev_loop->max_fd; i++ ) {
      ev_file_event_struct * file_event = &( ev_loop->file_events[ i ] ); 
      if ( EV_NONE == file_event->event_type ) continue;
      ev_fire_event_struct * fire_event = &( ev_loop->fire_events[ activity_fd_ret ] ); 
      if ( FD_ISSET( i, &fd_entry->_read_fds ) || FD_ISSET( i, &fd_entry->_write_fds ) ) {
        fire_event->fd         = i;
        fire_event->event_type = file_event->event_type;
        activity_fd_ret++; 
      } 
      //if ( FD_ISSET( i, &fd_entry->_write_fds ) ) {
      //}
    }
  }
  return activity_fd_ret;
}

/*
 * @desc   : get the IO/multiple name
 * @return : string
 */
static char * get_event_name() {
  return "select";
}
