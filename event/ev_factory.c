#include "../config.h"
#include "ev_factory.h"
#ifdef IS_EPOLL
#include "epoll.c"
#else
#include "select.c"
#endif

/*
 * @desc  : init the ev_loop_struct
 * @param : size, the size of normal event & fire event
 */
ev_loop_struct * init_ev( int size ) {
    ev_loop_struct * ev = ( ev_loop_struct * )malloc( sizeof( ev_loop_struct ) );
    ev->event_size = size;
    if ( NULL == ev ) {
        return NULL;
    }
    ev->file_events = ( ev_file_event_struct * )malloc( sizeof( ev_file_event_struct ) * size );
    if ( NULL == ev ) {
        return NULL;
    }
    ev->fire_events = ( ev_fire_event_struct * )malloc( sizeof( ev_fire_event_struct ) * size );
    if ( NULL == ev ) {
        return NULL;
    }
    ev_create_fd_entry( ev );
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
    file_event->event_type      = event_type;
    ev_loop->max_fd = fd;
    //printf( "ev_factory.c | ev_create_file_event | ev_loop->max_fd:%d\n", ev_loop->max_fd );
    // add fd to select\epoll\poll
    ev_add_event( ev_loop, fd, event_type );
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
        activity_fd_num = ev_poll( ev_loop );
        if ( activity_fd_num > 0 ) {
            for ( int i = 0; i < activity_fd_num; i++ ) {
                // take out the event for the fd from ev_loop->events
                ev_fire_event_struct * fire_event = &( ev_loop->fire_events[ i ] );
                //printf( "ev_factory.c | fire_event->fd:%d\n", fire_event->fd );
                ev_file_event_struct * file_event = &( ev_loop->file_events[ fire_event->fd ] );
                if ( EV_READABLE == fire_event->event_type ) {
                    file_event->read_processor( ev_loop, fire_event->fd );
                }
                else if ( EV_WRITEABLE == fire_event->event_type ) {

                }
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

/*
 * @desc   : get the IO/multiple name
 * @return : string
 */
static char * ev_get_name() {
    return ev_name();
}