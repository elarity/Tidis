// set a fd to non-blocking
void set_nonblock( int fd ) {
  int origin_fd_option = fcntl( fd, F_GETFL ); 
  int new_fd_option    = origin_fd_option | O_NONBLOCK; 
  fcntl( fd, F_SETFL, new_fd_option );
}

// trim
void ltrim( char *s ) {
  char *p;
  p = s;
  while( *p == ' ' || *p == '\t' ) {
    *p++;
  }
  strcpy( s, p );
}
void rtrim( char *s ) {
  int i;
  i = strlen( s ) - 1;
  while( ( ' ' == s[i] || s[i] == '\t' ) && i >= 0 ){
    i--;
  }
  s[ i+1 ] = '\0';
}
void trim( char *s ) {
  ltrim( s );
  rtrim( s );
}
