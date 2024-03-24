#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double  *results;
    int len;
}Data;

typedef struct tagQueueElement{
    struct tagQueueElement *next;
    struct tagQueueElement *prev;
    Data *data;
} QueueElement;

typedef struct  tagQueue{
    QueueElement *head;
    QueueElement *tail;
    int size;
}Queue;

void init(Queue *pqueue){
    pqueue->head = malloc(sizeof(QueueElement));
    pqueue->tail = malloc(sizeof(QueueElement));
    pqueue->head->next = pqueue->tail;
    pqueue->tail->prev = pqueue->head;
    pqueue->tail->next = NULL;
    pqueue->head->prev = NULL;
    pqueue->size = 0;
}

void free_queue(Queue *pqueue){
    QueueElement *curr = pqueue->head;
    while(curr != NULL){
        QueueElement *to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
}

void push_queue(Queue *pqueue, const Data *pdata){
    QueueElement *curr= (QueueElement*)malloc(sizeof(QueueElement));
    QueueElement *prev_el = pqueue->tail->prev;
    curr->data = pdata;
    prev_el->next = curr;
    pqueue->tail->prev = curr;
    curr->next = pqueue->tail;
    curr->prev = prev_el;
    pqueue->size++;
}

int peek(const Queue *pqueue, Data *pdata){
    if(pqueue->size == 0 ){
        return 0;
    }
    pdata = pqueue->head->next->data;
    return 1;
}

int pop( Queue *pqueue, Data *pdata){
    if(pqueue->size == 0){
        return -1;
    }
    pdata = pqueue->head->next->data;
    QueueElement *to_delete = pqueue->head->next;
    pqueue->head->next = to_delete->next;
    to_delete->next->prev = pqueue->head;
    pqueue->size--;
	free(to_delete);
    if(pqueue->size == 0){
        return 0;
    }
    return 1;
}

int get_total_count(const Queue *pqueue){
    QueueElement *curr = pqueue->head->prev;
    int res = 0;
    while (curr){
        res+=curr->data->len;
        curr=curr->next;
    }
    return res;
}

int main(){

    return 0;
}