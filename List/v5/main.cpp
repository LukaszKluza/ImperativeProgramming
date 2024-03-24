#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

//struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

void dump_list(const List *p_list) {
    ListElement *curr = p_list->head;
    while(curr){
        p_list->dump_data(curr->data);
        curr = curr->next;
    }
}

void dump_list_if(List *p_list, void *data) {
    ListElement *curr = p_list->head;
    while(curr){
        if(!p_list->compare_data(curr->data, data))
            p_list->dump_data(curr->data);
        curr = curr->next;
    }
}

void free_element(DataFp free_data, ListElement *to_delete) {
    free_data(to_delete->data);
    free(to_delete);
}

void free_list(List *p_list) {
    ListElement *curr = p_list->head;
    while (curr != NULL) {
        ListElement *to_delete = curr;
        if (p_list->free_data) p_list->free_data(to_delete->data);
        curr = curr->next;
        free(to_delete);
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

void push_front(List *p_list, void *data) {
    ListElement *element = (ListElement*)safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = p_list->head;
    p_list->head = element;
    if (!p_list->tail) p_list->tail = p_list->head;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = (ListElement*)safe_malloc(sizeof(ListElement));
    element->data = data;
    element->next = NULL;
    if (p_list->tail) p_list->tail->next = element;
    p_list->tail = element;
    if (!p_list->head) p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list) {
    if (!p_list->head) return;
    ListElement *curr = p_list->head;
    p_list->head = p_list->head->next;
    if (p_list->head == NULL) p_list->tail = NULL;
    free_element(p_list->free_data, curr);
}

void reverse(List *p_list) {
    ListElement *prev = NULL;
    ListElement *curr = p_list->head;
    ListElement *next = NULL;
    p_list->tail = p_list->head;
    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    p_list->head = prev;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement *element = p_list->head;
    ListElement *prev = NULL;

    /*while(element != NULL){
        if(p_list->compare_data(element->data, p_element->data) <= 0){
            prev = element;
        }
        element = element->next;
    }*/
    while(element != NULL && p_list->compare_data(element->data, p_element->data) <= 0){
        prev = element;
        element = element->next;
    }
    return prev;
}

void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *element = (ListElement*)data;

    if(previous == NULL){
        element->next = p_list->head;
        if(!p_list->head) p_list->tail = element;
        p_list->head = element;
    }
    else{
        element->next = previous->next;
        previous->next = element;
        if(previous == p_list->tail){
            p_list->tail = element;
        }
    }
}

void insert_in_order(List *p_list, void *p_data) {
    ListElement *element = (ListElement*)safe_malloc(sizeof(ListElement));
    element->data = p_data;
    element->next = NULL;
    ListElement *prev = find_insertion_point(p_list, element);
    if(prev == NULL || p_list->compare_data(element->data, prev->data) != 0){
        push_after(p_list, element, prev);
    }
    else{
        p_list->modify_data(prev->data);
    }
}
// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
    int *data=(int *)d;
    printf("%d ",*data);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    int *data1=(int *)a;
    int *data2=(int *)b;
    return *data1 - *data2;

}

int *create_data_int(int v) {
    int* data=(int*)malloc(sizeof(int));
    *data=v;
    return data;
}

// Word element


void dump_word (const void *d) {
    DataWord *data = (DataWord *)d;
    printf("%s ", data->word);
}

void free_word(void *d) {
    DataWord* data = (DataWord*)d;
    free(data->word);
    free(data);
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *data1=(DataWord *)a;
    DataWord *data2=(DataWord *)b;
    return strcmp(data1->word,data2->word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord* data1 = (DataWord*)a;
    DataWord* data2 = (DataWord*)b;
    return data1->counter - data2->counter;
}

void modify_word(void *p) {
    DataWord *data = (DataWord *)p;
    data->counter++;
}

void *create_data_word(const char *string, int counter) {
    DataWord *data = (DataWord*) safe_malloc(sizeof(DataWord));
    data->word = (char*) safe_malloc(strlen(string) + 1);
    data->counter = counter;
    strcpy(data->word, string);
    return data;
}


// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char buffer[BUFFER_SIZE];
    const char delimiters[] = " .,?!:;-\n\t\r";
    p_list->compare_data = cmp;
    while (fgets(buffer, BUFFER_SIZE, stream)) {
        char *token = strtok(buffer, delimiters);
        while (token) {
            if (cmp) {
                for (int i = 0; i < strlen(token); i++) {
                    token[i] = tolower(token[i]);
                }
                insert_in_order(p_list, create_data_word(token, 1));
            } else {
                push_back(p_list, create_data_word(token, 1));
            }
            token = strtok(NULL, delimiters);
        }
    }
}


// test integer list
void list_test(List *p_list, int n) {
    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word, NULL, NULL);
            stream_to_list(&list, stdin, NULL);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word, free_word, NULL, modify_word);
            stream_to_list(&list, stdin, cmp_word_alphabet);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
            dump_list_if(&list, &data);
            free_list(&list);
            break;
            //default:
              //  printf("NOTHING TO DO FOR %d\n", to_do);
    }
}
