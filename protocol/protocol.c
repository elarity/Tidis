/*
 * @desc : protocol parser
 * */
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"

command_cell_struct * create_command_array(unsigned int command_all_cell_length)
{
    command_cell_struct * r;
    r = create_command_cell(TIP_OBJECT_ARRAY);
    r->cell_length = command_all_cell_length;
    r->cell        = (command_cell_struct **)malloc(sizeof(command_cell_struct *) * command_all_cell_length);
    for (int i = 0; i < command_all_cell_length; i++) {
        r->cell[i] = NULL;
    }
    return r;
}

int decode_command(command_cell_struct ** command_cell, char * command)
{
    // offset. resp command cell is devided by \r\n
    //sleep( 2 );
    int temp_offset = 0;
    int offset;
    offset = process_offset(command);
    //printf( "---%s %d---\n\n\n", command, offset );
    //*command_cell = NULL;
    // *3$set\r\n$5user1\r\n$6wahaha\r\n
    //command = "*3$set\\r\\n$5user1\\r\\n$6wahaha\\r\\n";
    switch (command[0]) {
        case '*': {
            //printf( "* found.\n" );
            int command_all_cell_length;
            command_all_cell_length = string2int( command + 1 );
            //printf( "comcreate_command_arraymand length: %d\n", command_all_cell_length );
            //*command_cell = create_command_array( command_all_cell_length );
            // 递归调用，继续解析command
            for ( int i = 0; i < command_all_cell_length; i++ ) {
                //printf( "%s\n", command + offset );
                temp_offset = decode_command( &( ( *command_cell )->cell[ i ] ), command + offset );
                offset = offset + temp_offset;
            }
            //printf( "%p\n", *command_cell );
            //for ( int i = 0; i < command_all_cell_length; i++ ) {
                //printf( "%s\n", ( *command_cell )->cell[i]->string );
            //}
            break;
        }
        // 字符串
        case '$': {
            int command_cell_length;
            command_cell_length = string2int( command + 1 );
            //printf( "%c %c\n", command[ offset + command_cell_length ], command[ offset + command_cell_length + 1 ] );
            // 检测是否为\r\n 校验数据合法性 $3api\r\n
            //if ( command[ offset + command_cell_length ] != '\r' && command[ offset + command_cell_length + 1 ] != '\n' ) {
                //return -1;
            //}
            command_cell_struct * r = create_command_string( command, offset, command_cell_length );
            *command_cell = r;
            //printf( "---%s : %d : %d---\n\n\n", command, command_cell_length, offset );
            //offset = offset + command_cell_length + 2;
            break;
        }
    }
    return offset;
}

command_cell_struct * create_command_string(const unsigned char * command, unsigned int offset, unsigned int command_cell_length)
{
    //printf( "%s\n", command );
    command_cell_struct * string_cell = create_command_cell(TIP_OBJECT_STRING);
    string_cell->string = (unsigned char *)malloc( sizeof( unsigned char ) * command_cell_length);
    memcpy(string_cell->string, command + (offset - (command_cell_length + 2)), command_cell_length);
    string_cell->string[command_cell_length] = '\0';
    string_cell->string_length               = command_cell_length;
    //printf( "%s\n", string_cell->string );
    return string_cell;
}

command_cell_struct * create_command_cell(int type)
{
    command_cell_struct * cell = (command_cell_struct *)malloc(sizeof(command_cell_struct));
    cell->type          = type;
    cell->interger      = 0;
    cell->string        = NULL;
    cell->string_length = 0;
    return cell;
}

// convert string 2 int
int string2int(const unsigned char * string)
{
    int ret = 0;
    int neg = 0;
    // 处理负数.
    if ('-' == *string) {
        neg = 1;
        ++string;
    }
    while (*string >= '0' && *string <= '9') {
        ret = (ret * 10) + (*string - '0');
        if (ret > INT_MAX) {
            return -1;
        }
        ++string;
    }
    if (1 == neg) {
        ret = -ret;
    }
    return ret;
}

int process_offset(const unsigned char * command)
{
    int offset;
    for (offset = 1; offset < TIP_LINE_MAX_LEN; offset++) {
        if ('\n' == command[ offset ] && '\r' == command[ offset - 1 ]) {
            //printf( "process_offset : %d : %s\n", offset + 1, command );
            return offset + 1;
        }
    }
    return -1;
}