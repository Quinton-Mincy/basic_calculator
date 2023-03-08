#include "my_bc.h"


char* reverse_string(char* str){
    int endex = strlen(str) -1;
    int index = 0;
    while(index < endex){
        char temp = str[index];
        str[index] = str[endex];
        str[endex] = temp;
        index++;
        endex--;
    }
    return str;
}

char* itoa(int num){
    char str[1024] = {'\0'};
    int i = 0;
    bool neg = false;
    if(num<0){
        neg = true;
        num*=-1;
    }
    if(num != 0){
        while(num != 0){
            str[i] = (num%10) + '0';
            num/=10;
            i++;
        }
    }else{
        str[0] = 48;
    }

    if(neg){
        str[i] = '-';
    }
    char* final = strdup(str);
    if(strlen(final)>1 ){
        return reverse_string(final);
    }
    return final;
}

bool includes(char* operator){
    char* operators[] = {"+", "-", "*", "/", "%", "(", ")", NULL};
    int i = 0;
    while(operators[i] != NULL){
        if(strncmp(operators[i],operator,1) == 0){
            return true;
        }
        i++;
    }
    return false;
}

bool is_digit(char* digit){
    if(*digit>47 && *digit<58){
        // printf("%c\n",digit);
        return true;
    }
    return false;
}

bool is_num(char* num){
    int i = 0;
    if(num == NULL){
        return false;
    }
    if(num[i] == '-' && strlen(num)>1){
        i++;
    }
    while(num[i]!= '\0'){
        if(!is_digit(&num[i])){
            return false;
        }
        i++;
    }
    return true;
}