#include "../include/test.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "unistd.h"

unsigned int my_strlen(char *str){
    int i=0;
   
    while( *(str +i) != '\0')
        i++;
    
    return i;
}

int my_abs(double d){   
    if(d < 0)
        d = -d;
    
    return d;
}

char* my_itoa(int nmb){
    int i = 1;
    static char buf[sizeof(int)+2];
    char *res = buf + sizeof(int) + 1;

    do{
        *--res = '0' + (my_abs(nmb/i)%10);
        i *= 10;
    
    } while (nmb/i);
    
    if (nmb < 0)
        *--res = '-';
    
    return res;
}

int my_atoi(const char *nptr){
    long int i=0;
    int flag = 0;

    if(*nptr == '-'){
        nptr++;
        flag = 1;
    }

    while(*nptr != '\0'){
        if (*nptr >= '0' && *nptr <='9')
        {
            i *= 10;
            i += *nptr - '0';
        } else{
            break;
        }
        nptr++;
    }

    if(flag == 1)
        i= -i;
    
    return i;
}

int set_mode(const char **format, char **ch, va_list *args){
    char *str, *strPadding;
    int padding;
    char symbPadding;
    char *paddingSize;
    
    paddingSize = malloc(20 * sizeof(char));
    strPadding = paddingSize;
    symbPadding = 0;
    (*format)++;
    while(!(**format == ' ') && !(**format == '\0')){
        if(**format == '%'){
            *(*ch)++ ='%';
            (*format)++;
            break;
        }
        if(**format == '0'){
            symbPadding = '0';
            (*format)++;
        }
        while(**format >= '0' && **format <= '9'){
            *strPadding++  = *(*format)++;
        }
        padding = my_atoi(paddingSize);
        if(**format == 's')
            str = va_arg(*args, char *);
        if(**format == 'd')
            str = my_itoa(va_arg(*args, int));
        padding -= my_strlen(str);
        while(padding > 0){
            if(symbPadding)
                *(*ch)++ = symbPadding;
            else
                *(*ch)++ = ' ';
            padding--;
        }
        while(*str != '\0'){
            *(*ch)++ = *str++;
        }
        (*format)++;
        break;
    }
    return 0;
}

int my_printf(const char* format,...){
    va_list args;
    char *str, *res;
    str = malloc(256*sizeof(char));
    res = str;

    va_start(args, format);
    
    while(*format != '\0'){
        if(*format =='%'){
            set_mode(&format, &str, &args);
            continue;
        }
        *str = *format;
        str++;
        format++;
        
    }
    *str = '\0';
    write(1, res, my_strlen(res));
    va_end(args);
    free(res);

    return 0;
}
