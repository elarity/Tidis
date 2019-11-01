#include <sys/epoll.h>

/*
 * @desc : fd for select API
 */
typedef struct ev_fd_entry_struct {
  struct epoll_event * events; 
  int    epoll_fd;
} ev_fd_entry_struct;

/*
 * @desc : create epoll event for ev_fd_entry_struct
 */
static void ev_create_fd_entry( ev_loop_struct * ev_loop ) {
  int epoll_fd = 0;
  ev_fd_entry_struct * fd_entry = ( ev_fd_entry_struct * )malloc( sizeof( ev_fd_entry_struct ) ); 
  struct epoll_event events[ ev_loop->event_size ]; 
  fd_entry->events = events;
  epoll_fd = epoll_create( ev_loop->event_size ); 
  if ( epoll_fd < 0 ) {
    return;
  }
  fd_entry->epoll_fd = epoll_fd; 
  ev_loop->fd_entry  = fd_entry;
}

/*
 * @desc  : add a fd to fd_set
 * @param : ev_loop, the event-loop
 * @param : fd, socket fd
 * @param : event_type, EV_READABLE or EV_WRITEABLE
 */
static void ev_add_event( ev_loop_struct * ev_loop, int fd, int event_type ) {
  /*
   struct epoll_event {
     __uint32_t   events; 
     epoll_data_t data; 
   }; 
   typedef union epoll_data {
     void     * ptr; 
     int      fd;
     uint32_t uint32;
     uint64_t uint64;
   } epoll_data_t;
   */
  ev_fd_entry_struct * fd_entry;
  struct epoll_event event;
  fd_entry = ev_loop->fd_entry;
  event.data.fd = fd; 
  event.events  = EPOLLIN;
  event.events  |= EPOLLET;
  epoll_ctl( fd_entry->epoll_fd, EPOLL_CTL_ADD, fd, &event );
}

/*
 * @desc  : add a fd to fd_set
 * @param : ev_loop, the event-loop
 * @param : fd, socket fd
 * @param : event_type, EV_READABLE or EV_WRITEABLE
 */
static void ev_del_event( ev_loop_struct * ev_loop, int fd, int event_type ) {
  ev_fd_entry_struct * fd_entry;
  struct epoll_event event;
  fd_entry = ev_loop->fd_entry;
  epoll_ctl( fd_entry->epoll_fd, EPOLL_CTL_DEL, fd, &event );
}

/* 
 * @desc   : begin an event loop
 * @return : return activity fd
 */
static int ev_poll( ev_loop_struct * ev_loop ) {
  /*
   struct epoll_event {
     __uint32_t   events; 
     epoll_data_t data; 
   }; 
   typedef union epoll_data {
     void     * ptr; 
     int      fd;
     uint32_t uint32;
     uint64_t uint64;
   } epoll_data_t;
   */
  struct epoll_event   * event;
  ev_file_event_struct * file_event; 
  ev_fire_event_struct * fire_event; 
  ev_fd_entry_struct   * fd_entry;
  int activity_fd_num = 0;
  int activity_fd_ret = 0;
  fd_entry = ev_loop->fd_entry;
  //events   = fd_entry->events;
  activity_fd_num = epoll_wait( fd_entry->epoll_fd, fd_entry->events, ev_loop->event_size, -1 );
  //printf( "epoll.c : activity_fd_num : %d\n", activity_fd_num );
  if ( activity_fd_num > 0 ) {
    // loop the epoll-ready-event
    for ( int i = 0; i < activity_fd_num; i++ ) {
      event = &( fd_entry->events[ i ] );
      file_event = &( ev_loop->file_events[ event->data.fd ] ); 
      fire_event = &( ev_loop->fire_events[ activity_fd_ret ] );
      fire_event->event_type = file_event->event_type;
      fire_event->fd = event->data.fd;
      activity_fd_ret++;
    } 
  }
  return activity_fd_ret;
}

/*
 * @desc   : get the IO/multiple name
 * @return : string
 */
static char * ev_name() {
  return "epoll";
}
