#include <stddef.h>
unsigned long en_steg(unsigned long, unsigned char);
char de_steg(char []);
void print_binary(const void *ptr, size_t size);
void print_char(char []);
void rev(char *);
char *string_encode(char str_to_hide[],char str_being_coded[]);
char *get_extension(char str[],char ch);