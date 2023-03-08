#include "my_bc.h"

stack* init_stack(int size){//add an empty and full method to the stack and queue
    stack* new_stack = malloc(sizeof(stack));
    new_stack->size = size;
    new_stack->top = -1;
    new_stack->full_stack = &full_stack;
    new_stack->empty = &empty_stack;
    new_stack->push = &push;
    new_stack->pop = &pop;
    new_stack->peek = &peek;
    new_stack->arr = malloc(sizeof(char*)*size);
    return new_stack;
}

bool full_stack(stack* op_stack){
    return op_stack->count == op_stack->size;
}
void print_stack(stack* st){
    int i = 0;
    while(i<st->count){
        printf(" %s ",st->arr[i]);
        i++;
    }
    printf("\n");
}
void push(stack** op_stack, char* token){
    if( (*op_stack)->top < (*op_stack)->size - 1){
        (*op_stack)->top+=1;
        int top = (*op_stack)->top;
        (*op_stack)->arr[top] = strdup(token);
        (*op_stack)->count +=1;
        // printf("%d..%d..%s\n",(*op_stack)->top,top,token);
        // printf("^^%s^^\n",(*op_stack)->arr[top]);
    }else{
        printf("Stack Overflow!!\n");
    }
}

bool empty_stack(stack* op_stack){
    // printf("%d",op_stack->top);
    if(op_stack->count == 0){
        return true;
    }
    return false;
}

char* pop(stack** op_stack){//make more readbable
    char* token = NULL;
    // printf("hi\n");
    if((*op_stack)->top >= 0){
        token = (*op_stack)->arr[(*op_stack)->top];
        (*op_stack)->arr[(*op_stack)->top] = NULL;
        free((*op_stack)->arr[(*op_stack)->top--]);
        (*op_stack)->count -=1;
    // printf("hi\n");
    }else{
        printf("Stack Underflow!!\n");
    }
    return token;
}

char* peek( stack* op_stack){
    char* top = NULL;
    if(op_stack->count>0){
        // printf("")
        top = strdup(op_stack->arr[op_stack->top]);
    }
    return top;
}