#include "../include/test.h"
#include "stdio.h"

int my_strcmp(char *str1, char *str2)
{
    int flag = 0;
    for(int i = 0; str1[i]  != '\0' && str2[i] != '\0'; i++){
        if (str1[i] > str2[i]){
            flag = 1;
            break;
        }
        else
            if(str1[i] < str2[i]){
                flag = -1;
                break;
            }
    }
    return flag;
}
