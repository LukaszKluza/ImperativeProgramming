#include <stdio.h>

#define STACK_SIZE 10

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {
    int n =0 ;
    while(n<10 && stack[n] != 0)n++;
    if(n>9)return OVERFLOW;
    stack[n]=x;
    return OK;

}

int stack_pop(void) {
    int n =0 ;
    while(n<10 && stack[n] != 0)n++;
    if(n==0)return UNDERFLOW;
    int res = stack[n-1];
    stack[n-1]=0;
    return res;
}

int stack_state(void) {
    int n =0 ;
    while(n<10 && stack[n] != 0)n++;
    return n;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

void queue_print(void) {
    int n = 0;
    while(n<10 && queue[n] !=0){
        printf("%d ", queue[n]);
        n++;
    }
}

int queue_push(int in_nr) { // in_nr clients try to enter the queue
    int n = 0;
    while(n<10 && queue[n] !=0)n++;
    while(n<10 && in_nr > 0){
        curr_nr += 1;
        queue[n]=curr_nr;
        in_nr--;
        n++;
    }
    if(in_nr > 0){
        curr_nr += in_nr;
        return OVERFLOW;
    }
    return OK;
}


int queue_pop(int out_nr) {
    int n=0;
    while(n<10 && queue[n]!=0 && out_nr > 0){
        queue[n] = 0;
        n+=1;
        out_nr -= 1;
    }
    if(out_nr > 0)return UNDERFLOW;
    int k = 0;
    while(n<10){
        queue[k] = queue[n];
        queue[n] = 0;
        k++;
        n++;
    }
    return k-1;
}

int queue_state(void) {
    int n = 0;
    while(n<10 && queue[n] !=0)n++;
    return n;
}



// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;


int cbuff_push(int cli_nr) {
    int id = (out+len)%10;
    if(cbuff[id] != 0)return OVERFLOW;
    len += 1;
    cbuff[id] = cli_nr;
    return OK;
}

int cbuff_pop(void) {
    if(len == 0)return UNDERFLOW;
    len--;
    int res = cbuff[out];
    cbuff[out] = 0;
    out = (out+1)%10;
    return res;

}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    for(int i=out; i<10; ++i){
        if(cbuff[i]!=0)printf("%d ",cbuff[i]);
    }
    for(int i=0; i<out; ++i){
        if(cbuff[i]!=0)printf("%d ",cbuff[i]);
    }
}

int main(void) {
    int to_do, n, client_no, answer;
    scanf("%d", &to_do);
    switch(to_do) {
        case 1: // stack
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = stack_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", stack_pop());
                } else printf("\n%d\n", stack_state());
            } while(n != 0);
            break;
        case 2: // FIFO queue with shifts
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = queue_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
                } else {
                    printf("\n%d\n", queue_state());
                    queue_print();
                }
            } while(n != 0);
            break;
        case 3: // queue with cyclic buffer
            client_no = 0;
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", cbuff_pop());
                } else {
                    printf("\n%d\n", cbuff_state());
                    cbuff_print();
                }
            } while(n != 0);
            break;
        default:
            printf("NOTHING TO DO!\n");
    }
    return 0;
}