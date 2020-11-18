typedef struct tistr_struct {
    size_t free;
    size_t length;
    char * buf;
} tistr_struct;
tistr_struct * tistr_init(char *);
void tistr_truncate(tistr_struct *);
void tistr_empty(tistr_struct *);
void tistr_strcat(char *, char *);
void tistr_strcpy(char *, char *);
size_t tistr_get_length(tistr_struct *);
size_t tistr_get_free(tistr_struct *);
void tistr_expansion(tistr_struct *, size_t)