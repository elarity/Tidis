#include <stdio.h>
#include <string.h>
int main( int argc, char * argv[] ) {
    char msg[ 6 ] = "abc\0d";
    printf( "%s %ld %ld\n", msg, strlen( msg ), sizeof( msg ) );
    return 0;
}