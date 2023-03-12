#include "my_bc.h"
#include <readline/readline.h>

int calculate(char *expression){
    int ret;
    token_info* info = tokenizer(expression);
    if(info == NULL){
        return error_handle("parse error");
    }
    bool err = false;//will catch a divide by zero error in rev_pol
    if(is_valid(info)){//checks if mathematical expression is valid
        ret = rev_pol(shunting_yard(info),&err);
        if(err){//divide by zero error
            return error_handle("Runtime error: Divide by zero");
        }
        //evaluation successful
    }else{//expression not valid
        return error_handle("parse error");
    }
    return ret;
}

void read_loop(){
    char *buffer = NULL;
    while((buffer = readline(0))!=NULL){
        int solution = calculate(buffer);
        if(errno != 1){
            printf("%d\n",solution);
        }else{
            errno = 0;
        }
        free(buffer);
    }
}

int main(){
    read_loop();
    return EXIT_SUCCESS;
}