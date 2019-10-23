#ifndef __EV_H
#define __EV_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

#define MAX_FD_SIZE  1024
#define EV_NONE      0  // initial state of a fd
#define EV_READABLE  1  // when fd ready to read
#define EV_WRITEABLE 2  // when fd ready to write

// struct & data from ev_factory.c
struct ev_loop_struct;
typedef void file_event_processor( struct ev_loop_struct *, int );
// a file event struct
typedef struct ev_file_event {
  // Redis use mask code and bitwise to process event-type
  // But to lots of CURDER,it's a little wried
  // let's cubao a little,just use a int type to flag event-type
  int event_type;   
  file_event_processor * read_processor; 
  file_event_processor * write_processor; 
} ev_file_event_struct;
// a fire event struct
typedef struct ev_fire_event {
  int event_type;
  int fd; 
} ev_fire_event_struct;
// an event-loop struct
typedef struct ev_loop_struct {
  int max_fd; 
  int event_size;
  ev_file_event_struct * file_events; 
  ev_fire_event_struct * fire_events;
  //struct timeval tv_out;
  void * fd_entry; 
  void * tv_out;
} ev_loop_struct;

// func from ev_factory.c
ev_loop_struct * init_ev( int );
void ev_main( ev_loop_struct * );
int  ev_process( ev_loop_struct * );
void ev_create_file_event( ev_loop_struct *, int, file_event_processor *, int ); 

#endif
