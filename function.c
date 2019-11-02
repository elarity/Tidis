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
