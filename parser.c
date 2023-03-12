#include "my_bc.h"

int error_handle(char *err_msg, bool *no_err){
    perror(err_msg);
    *no_err = false;
    return EXIT_FAILURE;
}

char* lookahead(token* tokens, bool look_far){
    if(tokens->next->token == NULL){
        return NULL;
    }
    char* next_token;
    if(look_far){
        next_token = tokens->next->next->token;
    }else{
        next_token = tokens->next->token;
    }
    return next_token;
}



bool is_valid(token_info* info){
    token* tokens = info->tokens;
    int size = info->size;
    enum t_type last_token = -1;
    bool result = true;
    stack* paren_stack = init_stack(size);
    while(tokens!=NULL && result){
        char* curr_token = tokens->token;
        if(curr_token == NULL){
            break;
        }
        char* next_token = lookahead(tokens,false);
        if(is_num(curr_token)){
            if(last_token == DIGIT){
                result = false;
            }
            last_token = DIGIT;
        }else if(includes(curr_token)){
            if(*curr_token == '('){
                if(last_token == DIGIT || last_token == RIGHT_PAREN){
                    result = false;
                }
                last_token = LEFT_PAREN;

                paren_stack->push(&paren_stack,curr_token);
            }else if(*curr_token == ')'){
                if(last_token == LEFT_PAREN){
                    result = false;
                }else if( *(paren_stack->pop(&paren_stack)) != '('){
                    result = false;
                }
                last_token = RIGHT_PAREN;
            }else{
                bool next_is_num = is_num(next_token);
                if(last_token == OPERATOR && *curr_token!='-'){
                    result = false;
                }else if(last_token == MIN_OPERATOR){
                    if(*curr_token == '-' && !next_is_num){
                        result = false;
                    }
                }else if(*curr_token == '-'){
                    if(includes(next_token)){
                        if(*next_token != '-' && *next_token != '('){
                            result = false;
                        }
                    }
                    last_token = MIN_OPERATOR;
                }else{
                    last_token = OPERATOR;
                }
            }
            info->operators+=1;//need to fdix this whole function
        }else{
            result = false;
        }
        tokens = tokens->next;
    }
    if(last_token == OPERATOR || last_token == MIN_OPERATOR){
        result = false;
    }else if(!paren_stack->empty(paren_stack)){
        result = false;
    }
    return result;
}