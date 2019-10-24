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
  ev_fd_entry_struct * fd_entry;
  struct epoll_event event;
  fd_entry = ev_loop->fd_entry;
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
  ev_fd_entry_struct * fd_entry;
  int activity_fd_num = 0;
  int activity_fd_ret = 0;
  fd_entry = ev_loop->fd_entry;
  activity_fd_num = epoll_wait( fd_entry->epoll_fd, fd_entry->events, ev_loop->event_size, ev_loop->tv_out );
  if ( activity_fd_num > 0 ) {
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
