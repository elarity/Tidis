/*
 * This file is logic for net_base.c
 * If net_base.c is "model", this file(net_logic.c) is "logic"
 */
#include <stdbool.h>
#include "../main.h"
#include "net_base.c"

extern server_struct * server;

/*
 * @desc : set a fd to non-blocking
 */
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

/*
 * @desc : read command string from client.
 * */
void read_from_client( ev_loop_struct * ev_loop, int fd ) {
    //printf( "main.c | read_from_client \n" );
    size_t  recv_buf_length = 3000;
    char    recv_buf[ recv_buf_length ];
    ssize_t recv_length;
    recv_length = recv( fd, recv_buf, recv_buf_length, 0 );
    //printf( "%s", recv_buf );
    if ( -1 == recv_length ) {
        printf( "%s\n", strerror( errno ) );
        return;
    }

    //command_cell_struct * command;// = ( command_cell_struct * )malloc( sizeof( command_cell_struct * ) );
    //printf( "%p\n", command );
    char * command = "*3\r\n$3set\r\n$5user1\r\n$12wahahahahaha\r\n";
    //char * command = "*3\r\n$3get\r\n$5user1\r\n";
    int command_all_cell_length;
    command_all_cell_length = string2int( command + 1 );
    command_cell_struct * command_cell = create_command_array( command_all_cell_length );
    //command_cell_struct * command_cell = ( command_cell_struct * )malloc( sizeof( command_cell_struct * ) );
    //printf( "%p\n", command_cell );
    decode_command( &command_cell, command );
    //for ( int i=0; i < 3;i++ ) {
        //printf( "%d\n", command->cell_length );
        //printf( "%s\n", (command_cell->cell[ i ])->string );
    //}

    char * action = ( command_cell->cell[ 0 ] )->string;
    if ( 0 == strcmp( action, "set" ) ) {
        printf( "set action\n" );
        // 将解析后command中的数据转移到tidis object中去，然后释放command占用的内存
        // 一、从command中分配到tidis object中
        tidis_object_struct * key_object = ( tidis_object_struct * )malloc( sizeof( tidis_object_struct ) );
        key_object->type = TIDIS_OBJECT_STRING;
        //key_object->ptr  = ( char * )malloc( sizeof(  ) );
        key_object->ptr = (command_cell->cell[ 1 ])->string;
        tidis_object_struct * value_object = ( tidis_object_struct * )malloc( sizeof( tidis_object_struct ) );
        value_object->type = TIDIS_OBJECT_STRING;
        //key_object->ptr  = ( char * )malloc( sizeof(  ) );
        value_object->ptr = (command_cell->cell[ 2 ])->string;
        //printf( "%s\n", (char *)key_object->ptr );
        //printf( "%s\n", (char *)value_object->ptr );
        // 二、存储到hashtable中.
        //printf( "%d\n", server->db_num );
        ht_st * ht_key = server->db->key;
        add_ht_node( ht_key, key_object, value_object );


        printf( "key : %s\n", (char *)key_object->ptr );
        ht_node_st * ht_node = get_ht_node( ht_key, key_object );
        //printf( "%s\n", ht_node->key );
        tidis_object_struct * value_object1 = (tidis_object_struct *)ht_node->value;
        printf( "value : %s\n", (char *)value_object1->ptr );

    } else if ( 0 == strcmp( action, "get" ) ) {
        printf( "get action\n" );
        // 一、从command中分配到tidis object中
        tidis_object_struct * key_object = ( tidis_object_struct * )malloc( sizeof( tidis_object_struct ) );
        key_object->type = TIDIS_OBJECT_STRING;
        //key_object->ptr  = ( char * )malloc( sizeof(  ) );
        key_object->ptr = (command_cell->cell[ 1 ])->string;
        ht_st * ht_key = server->db->key;
        printf( "key : %s\n", (char *)key_object->ptr );
        ht_node_st * ht_node = get_ht_node( ht_key, key_object );
        printf( "%s\n", ht_node->key );
        //tidis_object_struct * value_object = (tidis_object_struct *)ht_node->value;
        //printf( "%d\n", value_object->type );
    }


    //send( fd, "helloclient", 12, 0 );
}
