#include <stdio.h>
#include "steg.h"
unsigned long en_steg(unsigned long str,unsigned char chr)
    {   
        str=str & 0xfefefefefefefefe;//clear last bit of byte
        unsigned long mask=0;
        for(int i=0;i<8;i++)
            {
                unsigned  long temp= ((1<<i) & chr);
                temp = temp << ((i*8)-i);
                mask =mask | temp; 
            }
        rev((char*)&mask);//for proper alignment
        str=str | mask;
        return str;
    }
char de_steg(char str[])
    {
        char ch=0;
        for(int i=0;i<8;i++)
            ch = ch | ((str[i] & 1)<<(7-i));
        return ch;
    }
void print_binary(const void *ptr, size_t size) {
    const unsigned char *bytes = ptr;  // raw memory
    for (size_t i = 0; i < size; i++) {
        unsigned char byte = bytes[size - 1 - i]; // MSB first
        for (int bit = 7; bit >= 0; bit--) {
            putchar((byte >> bit) & 1 ? '1' : '0');
            if (bit % 4 == 0) putchar(' '); // space every 4 bits
        }
    }
    putchar('\n');
}
void rev(char *str)
    {
        for(int i=0;i<4;i++)
            {
                char temp=str[i];
                str[i]=str[7-i];
                str[7-i]=temp;
            }
    }
void print_char(char str[])
    {
        for(int i=0;i<8;i++)
            printf("%hhx ",str[i]);
        printf("\n");
    }
char *string_encode(char str_to_hide[],char str_being_coded[])
    {
        unsigned long *hold=(unsigned long*)str_being_coded;
        while(*str_to_hide!='\0')
            {
                *hold=en_steg(*hold,*str_to_hide);
                str_to_hide++;
                hold++;
            }
        return str_being_coded;
        ///not to be used with anything other than pure strings.
    }
char *get_extension(char str[],char ch)
    {
        while(*str!='\0')
            {
                if(*str==ch)
                    return (str+1);
                str++;
            }
    }