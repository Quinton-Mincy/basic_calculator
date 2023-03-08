#include "my_bc.h"

token_info* tokenizer(char* expression){
    int i = -1;
    int j = 0;
    token_info* info = NULL;
    token* head = create_node(NULL,0,&info);;
    token* tail = head;
    char buffer[32] = {'\0'};
    while(expression[++i] != '\0'){
        if(is_digit(&expression[i])){
            buffer[j++] = expression[i];
        }else if(includes(&expression[i])){
            if(buffer[0] != '\0'){//checks if their are 2 operators in a row
                add_node(&tail, &info, &buffer[0],j);
                bzero(buffer,j);
                j = 0;
            }
            add_node(&tail, &info, &expression[i],1);
        }else{//skips white spaces, or returns NULL if a token is invalid
            if(expression[i] != ' '){
                return NULL;
            }
        }
    }
    if(buffer[0] != '\0'){
        add_node(&tail, &info, &buffer[0],j);
    }
    add_node(&tail, &info, NULL,0);//end with NULL
    return info;
}

queue* shunt_yard(token_info* info){
    bool skip = false;
    char* curr_token;
    int size = info->size;
    queue* output = init_queue(size);
    stack* operator_stack = init_stack(info->operators);
    token* tokens = info->tokens;
    while( (curr_token = tokens->token) != NULL){
        // printf("%s\n",curr_token);
        if(skip){
            skip = false;
            tokens = tokens->next;
            continue;
        }
        if(is_num(curr_token)){
            output->enqueue(&output, curr_token);
        }else{
            char* next_token = lookahead(tokens,false);
            if(*curr_token == ')' && next_token == NULL){//ending right paren at end of expression
                handle_paren(&output,&operator_stack);
                break;
            }
            char* next_next_token = lookahead(tokens,true);
            if( (next_next_token == NULL && *curr_token == ')') && *next_token == ')' ){//needs fixing
                handle_paren(&output,&operator_stack);
                break;
            }
            if(includes(next_token)){
                if( (*curr_token!= '-' && *next_next_token == '(') && *curr_token !='(' ){
                    operator_stack->push(&operator_stack,curr_token);
                }
                else if(*curr_token == '-' && *next_token == '('){
                    char* neg_one = "-1";
                    char* multiply = "*";
                    output->enqueue(&output,neg_one);
                    operator_stack->push(&operator_stack,multiply);
                }
                else if(is_num(next_next_token)){
                    if( *curr_token != '-'){
                        operator_stack->push(&operator_stack,curr_token);
                    }
                    if(*next_token != '('){
                        skip = true;
                        int temp = atoi(next_next_token);
                        temp*=-1;
                        char* back_2_str = itoa(temp);
                        lookahead_mod(&tokens,true, back_2_str);
                    }
                }
                else{
                    // printf("hi\n");
                }
            }
            else if(*curr_token!=')'){
                if(!operator_stack->empty(operator_stack)){
                    swap_top(&output,&operator_stack,curr_token);
                }
                operator_stack->push(&operator_stack,curr_token);
                // print_stack(operator_stack);
            }
            else{
                handle_paren(&output,&operator_stack);
            }
        }
        // printf("%s\n",(operator_stack)->peek(operator_stack));
        // printf("%s\n",operator_stack->peek(operator_stack));
        tokens = tokens->next;
    }
    if(!operator_stack->empty(operator_stack)){
        push_rem_ops(&output,&operator_stack);
    }
    // printf("%d\n",output->max_size);
    // print_queue(output);
    return output;
}

int main(){
    // char* express = "55 +3*-2 *(3+12)";
    char* express = "22*5/(3/-3)";
    // char* express = "312/0";
    // char* express = "321()";
    // char* express = "1--2(3-4)";
    // char* express = "2 +    5  +3+  2 - 3";
    // char* express = "3";
    // char* express = "22*5/(3+2)";
    // char* express = "-(-((-4)+-6))";
    // char* express = "22*5/(3+2)";
    // char* express = "2+3--4+5";
    // char* express = "2+2(6-5)";
   
    printf("%s\n",express);
    token_info* info = tokenizer(express);
    token* tokens = info->tokens;

    if(is_valid(tokens,info)){
        printf("%d\n",rev_pol(shunt(info)));
    }else{
        printf("parse error\n");
    }
    return 0;
}