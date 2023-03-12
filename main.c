#include "my_bc.h"

int main(int ac, char* av[]){
    if(ac != 2){
        return -1;
    }
    int ret;
    char* express = av[1];
    token_info* info = tokenizer(express);
    if(info == NULL){
        errno = 1;
        perror("parse error");
        return 1;
    }
    bool err = false;//will catch a divide by zero error in rev_pol
    if(is_valid(info)){//checks if mathematical expression is valid
        ret = rev_pol(shunt(info),&err);
        if(err){//divide by zero error
            return 1;
        }
        printf("%d\n",ret);//evaluation successful
    }else{//expression not valid
        errno = 1;
        perror("parse error");
        return 1;
    }
    return EXIT_SUCCESS;
}