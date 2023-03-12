#include "my_bc.h"

int main(int ac, char* av[]){
    if(ac != 2){
        return -1;
    }
    int ret;
    char* express = av[1];
    token_info* info = tokenizer(express);
    if(info == NULL){
        return error_handle("parse error");
    }
    bool err = false;//will catch a divide by zero error in rev_pol
    if(is_valid(info)){//checks if mathematical expression is valid
        ret = rev_pol(shunting_yard(info),&err);
        if(err){//divide by zero error
            return error_handle("Runtime error: Divide by zero");
        }
        printf("%d\n",ret);//evaluation successful
    }else{//expression not valid
        return error_handle("parse error");
    }
    return EXIT_SUCCESS;
}