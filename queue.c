#include "my_bc.h"

queue* init_queue(int max_size){
    queue* new_queue = malloc(sizeof(queue));
    new_queue->max_size = max_size;
    new_queue->count = 0;
    new_queue->front = 0;
    new_queue->rear = -1;
    new_queue->full_queue = &full_queue;
    new_queue->empty = &empty_queue;
    new_queue->enqueue = &enqueue;
    new_queue->dequeue = &dequeue;
    new_queue->arr = malloc(sizeof(char*)*max_size);
    return new_queue;
}

bool full_queue(queue* output_queue){
    return output_queue->count == output_queue->max_size;
}

bool empty_queue(queue* output_queue){
    return output_queue->count == 0;
}

void print_queue(queue* output){
    int size = output->count;
    // printf("%d\n",size);
    while(size>0){
        printf("%s -> ",dequeue(output));
        size--;
    }
    printf("\n");
}

void enqueue(queue** output_queue, char* token){
    int max_size = (*output_queue)->max_size;
    int count = (*output_queue)->count;
    int rear = (*output_queue)->rear;
    if(count < max_size){
        (*output_queue)->rear = (rear+=1)%max_size; 
        (*output_queue)->arr[rear] = strdup(token);
        (*output_queue)->count++;
    }else{
        printf("Queue is Overflow!!\n");
    }
}

char* dequeue(queue* output_queue){
    int front = output_queue->front;
    int max_size = output_queue->max_size;
    char* token = NULL;
    if(output_queue->count > 0){
        token = output_queue->arr[front];
        output_queue->front = (front+1)%max_size;
        output_queue->count--; 

    }else{
        printf("Queue Underflow!!\n");
    }
    return token;
}