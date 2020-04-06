#define TIP_LINE_MAX_LEN 1024

#define TIP_OBJECT_ARRAY  1
#define TIP_OBJECT_STRING 2
#define TIP_OBJECT_INT    3

/*
 * @desc : protocol parser
 * */
typedef struct command_cell_struct {
    int type;      // command cell type
    int length;    // command cell length
    int interger;  //
    unsigned int  string_length;   // bulk string length
    unsigned char * string;        // bulk string
    unsigned int  cell_length;
    struct command_cell_struct ** cell; // next command cell part
} command_cell_struct;

// function prototpye
int decode_command( command_cell_struct **, char * );              // encode whole command
void encode_command();                            // decode whole command
command_cell_struct * create_command_cell();      // create a command_cell_struct
void free_command_cell( command_cell_struct * );  // release useless memory
int string2int( const unsigned char * );         // convert string 2 int
char * int2string();                             // convert int 2 string
int process_offset( const unsigned char * );     // calculate offset \r\n
command_cell_struct * create_command_array( unsigned int );
command_cell_struct * create_command_string( const unsigned char *, unsigned int, unsigned int );