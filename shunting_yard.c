#include "my_bc.h"

int add(int op1, int op2){
    return op1+op2;
}

void base_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip){
    if(is_num(prev_token) || *prev_token == ')'){
        if(!is_num(next_token)){//can only be - or (
            if(*next_token == '-'){
                min_to_neg(tokens, true, skip);
                // return;
            }
        }
    }
    // (*op_stack)->push(op_stack,curr_token);
    swap_top(output,op_stack,curr_token);
}

int divide(int op1, int op2){
    return op1/op2;
}

void handle_paren(queue** output, stack** operators){
    while( *((*operators)->peek(*operators)) != '(' && !(*operators)->empty(*operators)){
        (*output)->enqueue(output,(*operators)->pop(operators));
    }
    (*operators)->pop(operators);//pop the left paren from stack
}

operation_hash* hash_operations(eq_tuples ops[]){
    int i = 0;
    operation_hash* op_hash = malloc(sizeof(operation_hash));
    while(ops[i].operator != -1){
        op_hash->hash[(int)ops[i].operator] = ops[i];
        i++;
    }
    return op_hash;
}

tuple_hash* hash_tuples(tuple op_tuples[]){
    int i = 0;
    tuple_hash* hash = malloc(sizeof(tuple_hash));
    while(op_tuples[i].operator != -1){
        hash->hash[(int)op_tuples[i].operator] = op_tuples[i];
        i++;
    }
    return hash;
}

void l_paren_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip){
    if(output){};
    if(tokens){};
    if(prev_token){};
    if(next_token){};
    if(skip){};
    (*op_stack)->push(op_stack,curr_token);
}

void min_to_neg(token** tokens, bool look_far, bool* skip){
    int temp;
    char* back_2_str;
    if(look_far){
        *skip = true;
        temp = atoi(lookahead(*tokens,true));
        temp*=-1;
        back_2_str = itoa(temp);
        lookahead_mod(tokens,true,back_2_str);
    }else{
        temp = atoi(lookahead((*tokens),false));
        temp*=-1;
        back_2_str = itoa(temp);
        lookahead_mod(tokens,false,back_2_str);
    }
}

void min_to_neg_one(queue** output, stack** op_stack){
    char* neg_one = "-1";
    char* multiply = "*";
    (*output)->enqueue(output,neg_one);
    (*op_stack)->push(op_stack,multiply);
}

void minus_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip){
    if(is_num(prev_token)){
        if(!is_num(next_token)){//can only be - or ( / )
            if(*next_token == '-'){
                min_to_neg(tokens, true, skip);
            }
        }
        swap_top(output,op_stack,curr_token);
        // (*op_stack)->push(op_stack,curr_token);
    }else{//will be ( or ), or the first token in expression
        if( (prev_token == NULL || *prev_token == ')') || *prev_token == '(' ){
            if(!is_num(next_token)){
                min_to_neg_one(output,op_stack);
            }else{
                min_to_neg(tokens, false,skip);
                
            }
        }
    }
}

int modulus(int op1, int op2){
    return op1%op2;
}
int multiply(int op1, int op2){
    return op1*op2;
}

void push_rem_ops(queue** output, stack** operators){
    while(!(*operators)->empty(*operators)){
        (*output)->enqueue(output,(*operators)->pop(operators));
    }
}

int rev_pol(queue* postfix, bool* err){
    stack* result = init_stack(postfix->count);
    eq_tuples ops[] = { {'+',&add}, {'-',&subtract}, {'*',&multiply}, {'/',&divide}, {'%',&modulus}, {-1,NULL} };
    operation_hash* hash = hash_operations(ops);
    while(!postfix->empty(postfix)){
        char* curr_elem = postfix->dequeue(postfix);
        if(!is_num(curr_elem)){
            int operand2 = atoi(result->pop(&result));
            int operand1 = atoi(result->pop(&result));
            if(*curr_elem == '/' && operand2 == 0){
                errno = 1;
                *err = true;
                perror("divide by zero\n");
                return -1;
            }
            int answer = send_eq(operand1, operand2,hash->hash[(int)*curr_elem].operation);
            // printf("%d %c %d = %d\n",operand1,*curr_elem,operand2,answer);
            result->push(&result,itoa(answer));
        }else{
            result->push(&result,curr_elem);
        }
    }
    return atoi(result->pop(&result));
}

void r_paren_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip){
    if(tokens){};
    if(prev_token){};
    if(next_token){};
    if(curr_token){};
    if(skip){};
    handle_paren(output,op_stack);
}

queue* shunting_yard(token_info* info){
    queue* output = init_queue(info->size);
    stack* op_stack = init_stack(info->size);
    token* tokens = info->tokens;
    tuple op_tuples[] = { {'+',0,&base_rules}, {'-',0,&minus_rules}, {'*',1,&base_rules}, {'/',1,&base_rules}, {'%',1,&base_rules}, {'(',2,&l_paren_rules}, {')',2,&r_paren_rules}, {-1,-1,NULL} };
    tuple_hash* hash = hash_tuples(op_tuples);
    char* curr_token = NULL;
    char* prev_token = NULL;
    char* next_token = NULL;
    bool skip = false;
    while( (curr_token = tokens->token) != NULL ){
        if(skip){
            skip = false;
            tokens = tokens->next;
            continue;
        }
        next_token = lookahead(tokens,false);
        if(!is_num(curr_token)){
            hash->hash[(int)(*curr_token)].cmd(&op_stack, &output, &tokens, curr_token, prev_token,next_token,&skip);
        }else{
            output->enqueue(&output,curr_token);
        }
        prev_token = curr_token;//should be here in case of a skip. then prev token will be correct
        tokens = tokens->next;
    }
    push_rem_ops(&output, &op_stack);
    return output;
}

int subtract(int op1,int op2){
    return op1-op2;
}

void swap_top(queue** output, stack** operators, char* token){
    tuple op_tuples[] = { {'+',0,NULL}, {'-',0,NULL}, {'*',1,NULL}, {'/',1,NULL}, {'%',1,NULL}, {'(',2,NULL}, {')',2,NULL}, {-1,-1,NULL} };
    int tup_index = which_tup(*token);
    while(!(*operators)->empty(*operators)){
        if( (op_tuples[tup_index].priority <= op_tuples[which_tup(*(*operators)->peek(*operators))].priority) && *((*operators)->peek(*operators)) != '(' ){
            (*output)->enqueue(output,(*operators)->pop(operators));
        }
        else{
            break;
        }
    }
    (*operators)->push(operators,token);
}

int send_eq(int op1, int op2, int (*some_operation) (int val1, int val2)){
    return some_operation(op1,op2);
}

int which_tup(char operator){
    tuple op_tuples[] = { {'+',0,NULL}, {'-',0,NULL}, {'*',1,NULL}, {'/',1,NULL}, {'%',1,NULL}, {'(',2,NULL}, {')',2,NULL}, {-1,-1,NULL} };
    int i = 0;
    char curr_op;
    while( (curr_op = op_tuples[i].operator) != -1){
        if(operator == curr_op){
            // printf("$$%d$$\n",i);
            return i;
        }
        i++;
    }
    return -1;
}