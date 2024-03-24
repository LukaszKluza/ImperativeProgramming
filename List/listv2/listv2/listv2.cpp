#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>


#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement* next;
    void* data;
} ListElement;

typedef struct {
    ListElement* head;
    ListElement* tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void* safe_strdup(const char* string) {
    void* ptr = strdup(string);
    if (ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List* p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = 0;
    p_list->tail = 0;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement* curr = p_list->head;
    while (curr != NULL) {
        p_list->dump_data(curr->data);
        curr = curr->next;
    }
    _CrtDumpMemoryLeaks();
}

// Print elements of the list if comparable to data
void dump_list_if(List* p_list, void* data) {
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement* to_delete) {
}

// Free all elements of the list
void free_list(List* p_list) {
    ListElement* curr;
    while (p_list->head) {
        curr = p_list->head;
        p_list->head = p_list->head->next;
        free(curr);
    }
    free(p_list);
}

// Push element at the beginning of the list
void push_front(List* p_list, void* data) {
    ListElement* element = (ListElement*)malloc(sizeof(ListElement));
    element->next = p_list->head;
    element->data = data;
    if (p_list->head != 0) {
        p_list->head = element;
    }
    else {
        p_list->head = p_list->tail = element;
    }
}

// Push element at the end of the list
void push_back(List* p_list, void* data) {
    ListElement* element = (ListElement*)malloc(sizeof(ListElement));
    element->next = 0;
    element->data = data;
    if (p_list->tail != 0) {
        p_list->tail->next = element;
        p_list->tail = element;
    }
    else {
        p_list->head = p_list->tail = element;
    }
}

// Remove the first element
void pop_front(List* p_list) {
    if (p_list->head != 0) {
        ListElement* curr = p_list->head;
        p_list->head = p_list->head->next;
        // free(curr);
    }
}

// Reverse the list
void reverse(List* p_list) {
    ListElement* prev = 0;
    ListElement* curr = p_list->head;
    ListElement* next = 0;
    p_list->tail = p_list->head;
    while (curr != 0) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    p_list->head = prev;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List* p_list, ListElement* p_element) {
    ListElement* prev = 0, * head = p_list->head;
    while (head != 0 && head->data < p_element->data) {
        prev = head;
        head = head->next;
    }
    return prev;
}

// Insert element after 'previous'
void push_after(List* p_list, void* data, ListElement* previous) {
    ListElement* new_el = (ListElement*)malloc(sizeof(ListElement));
    new_el->data = data;
    new_el->next = previous->next;
    previous->next = new_el;
}

// Insert element preserving order
void insert_in_order(List* p_list, void* p_data) {
    ListElement* new_el = (ListElement*)malloc(sizeof(ListElement));
    new_el->data = p_data;
    ListElement* prev = find_insertion_point(p_list, new_el);
    if ((prev != 0 && prev->next != 0 && prev->next->data == new_el->data) || (p_list->head != 0 && p_list->head->data == p_data)) {
        free(new_el);
        return;
    }
    else if (prev == 0) {
        push_front(p_list, p_data);
    }
    else {
        push_after(p_list, new_el->data, prev);
    }
    //free(new_el);
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void* d) {
    const int* val = (const int*)d;
    printf("%d ", val);
}

void free_int(void* d) {
}

int cmp_int(const void* a, const void* b) {
}

int* create_data_int(int v) {
}

// Word element

typedef struct DataWord {
    char* word;
    int counter;
} DataWord;

void dump_word(const void* d) {
}

void dump_word_lowercase(const void* d) {
}

void free_word(void* d) {
}

int cmp_word_alphabet(const void* a, const void* b) {
}

int cmp_word_counter(const void* a, const void* b) {
}

void modify_word(void* p) {
}

void* create_data_word(const char* string, int counter) {
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List* p_list, FILE* stream, CompareDataFp cmp) {
}



// test integer list
void list_test(List* p_list, int n) {
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

    scanf("%d", &to_do);
    switch (to_do) {
    case 1: // test integer list
        scanf("%d", &n);
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
        scanf("%d", &n);
        init_list(&list, dump_word_lowercase, free_word, NULL, modify_word);
        stream_to_list(&list, stdin, cmp_word_alphabet);
        list.compare_data = cmp_word_counter;
        DataWord data = { NULL, n };
        dump_list_if(&list, &data);
        free_list(&list);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}

