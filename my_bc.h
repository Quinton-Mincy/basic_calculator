#ifndef MY_BC_H
#define MY_BC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

enum t_type{
    DIGIT,
    LEFT_PAREN,
    RIGHT_PAREN,
    OPERATOR,
    MIN_OPERATOR
};

typedef struct s_queue{
    int count;
    int max_size;
    int front;
    int rear;
    void  (*enqueue)    (struct s_queue** this, char* token);
    char* (*dequeue)    (struct s_queue* this);
    bool  (*full_queue) (struct s_queue* this);
    bool (*empty)       (struct s_queue* this);
    char** arr;
}queue;

typedef struct s_stack{
    int count;
    int size;
    int top;
    bool (*full_stack) (struct s_stack* this);
    bool (*empty) (struct s_stack* this);
    void (*push)  (struct s_stack** this, char* token);
    char* (*pop)  (struct s_stack** this);
    char* (*peek) (struct s_stack* this);
    char** arr;
}stack;

typedef struct s_token{
    char* token;
    struct s_token* next;
}token;

typedef struct s_token_info{
    int size;
    int operators;
    struct s_token* tokens;
}token_info;

typedef struct s_p_tuple{
    char operator;
    int  priority;
    void (*cmd) (struct s_stack** op_stack, struct s_queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip);
}tuple;

typedef struct s_tuple_hash{
    struct s_p_tuple hash[127];
}tuple_hash;

typedef struct s_eq_tuples{
    char operator;
    int (*operation) (int op1, int op2);  
}eq_tuples;

typedef struct s_operation_hash{
    struct s_eq_tuples hash[127];
}operation_hash;

//stack.c
bool full_stack(stack* op_stack);
stack* init_stack(int size);
bool empty_stack(stack* op_stack);
void push(stack** op_stack, char* token);
char* pop(stack** op_stack);
char* peek( stack* op_stack);
void print_stack(stack* st);

//queue.c
queue* init_queue(int max_size);
void print_queue(queue* output);
void enqueue(queue** output_queue, char* token);
char* dequeue(queue* output_queue);
bool full_queue(queue* output_queue);
bool empty_queue(queue* output_queue);

//tokens.c
token_info* init_token_info(token* token_head);
token* create_node(char* new_token, int len,token_info** info);
void print(token** test);
void print_ll(token* tokens);
void dup_token(token** node, char* token, int len);
void add_node(token** node, token_info** info, char* token, int len);
void lookahead_mod(token** tokens, bool look_far, char* val);
token_info* tokenizer(char* expression);

//parser.c
char* lookahead(token* tokens, bool look_far);
bool is_valid(token_info* info);


//helper.c
bool includes(char* operator);
bool is_digit(char* digit);
bool is_num(char* num);
char* reverse_string(char* str);
char* itoa(int num);

//shunt_yard.c
queue* shunt(token_info* info);
void swap_top(queue** output, stack** operators, char* token);
void handle_paren(queue** output, stack** operators);
int which_tup(char operator);
void push_rem_ops(queue** output, stack** operators);
int rev_pol(queue* postfix, bool* err);
int send_eq(int op1, int op2, int (*some_operation) (int val1, int val2));
operation_hash* hash_operations(eq_tuples ops[]);
int add(int op1, int op2);
int subtract(int op1,int op2);
int multiply(int op1, int op2);
int divide(int op1, int op2);
int modulus(int op1, int op2);

void r_paren_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip);
void swap_top(queue** output, stack** operators, char* token);
void l_paren_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip);
void minus_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip);
void min_to_neg_one(queue** output, stack** op_stack);
void base_rules(stack** op_stack, queue** output, token** tokens, char* curr_token, char* prev_token, char* next_token, bool* skip);
void push_rem_ops(queue** output, stack** operators);
void min_to_neg(token** tokens, bool look_far, bool* skip);
tuple_hash* hash_tuples(tuple op_tuples[]);

#endif