#include "my_bc.h"
#include <readline/readline.h>

int calculate(char *expression, bool *no_err){
    int ret;
    token_info* info = tokenizer(expression);
    if(info == NULL){
        return error_handle("parse error",no_err);
    }
    bool err = false;//will catch a divide by zero error in rev_pol
    if(is_valid(info)){//checks if mathematical expression is valid
        ret = rev_pol(shunting_yard(info),&err);
        if(err){//divide by zero error
            return error_handle("Runtime error: Divide by zero",no_err);
        }
        //evaluation successful
    }else{//expression not valid
        return error_handle("parse error",no_err);
    }
    return ret;
}

void read_loop(){
    printf("**********************************************************************\n\
* Thanks for trying out my calculator!                               *\n*                                                                    *\n\
* Currently supports: addition, subtraction, multiplication, division, modulo*\n\
**********************************************************************\n\n");
    char *buffer = NULL;
    bool no_err = true;
    while((buffer = readline(0))!=NULL){
        if(strncmp(buffer,"quit",4)==0){
            free(buffer);
            return;
        }
        else if(strlen(buffer)){
            int solution = calculate(buffer,&no_err);
            if(no_err){
                printf("%d\n",solution);
            }else{
                no_err = true;
            }
        }
        free(buffer);
    }
}

int main(){
    read_loop();
    return EXIT_SUCCESS;
}