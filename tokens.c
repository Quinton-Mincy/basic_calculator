#include "my_bc.h"

token_info* init_token_info(token* token_head){
    token_info* info = malloc(sizeof(token_info));
    info->size = 0;
    info->operators = 0;
    info->tokens = token_head;
    return info;
}

void dup_token(token** node, char* token, int len){
    (*node)->token = strndup(token, len);
}

void lookahead_mod(token** tokens,bool look_far, char* val){
    char* temp;
    if(look_far){
        temp = (*tokens)->next->next->token;
        free(temp);
        (*tokens)->next->next->token = strdup(val);
    }else{
        temp = (*tokens)->next->token;
        free(temp);
        (*tokens)->next->token = strdup(val);
    }
}

void add_node(token** node, token_info** info, char* token, int len){
    if( (*node)->token == NULL){
        dup_token(node,token,len);
    }else{
        (*node)->next = create_node(token,len,info);
        (*node) = (*node)->next;
    }
}

token* create_node(char* new_token, int len,token_info** info){
    token* new_node = malloc(sizeof(token));
    if(new_token == NULL){
        new_node->token = NULL;
    }else{
        // new_node->token = strndup(new_token,len);
        dup_token(&new_node,new_token,len);
    }
    new_node->next = NULL;
    if( *info == NULL){
        *info = init_token_info(new_node);
    }
    (*info)->size++;
    return new_node;
}


void print(token** test){
    while(*test != NULL){
        printf("%s\n",(*test)->token);
        *test = (*test)->next;
    }
}

void print_ll(token* tokens){
    while(tokens!=NULL){
        if(tokens->next!=NULL){
            printf("%s -> ",tokens->token);
        }else{
            printf("%s",tokens->token);
        }
        tokens=tokens->next;
    }
    printf("\n");
}