#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->data = malloc(block_size*element_size);
    vector->size = 0;
    vector->element_size = element_size;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if(new_capacity > vector->capacity){
        void * new_data = realloc(vector->data, new_capacity*vector->element_size);
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if(new_size > vector->size){
        size_t diff_size = (new_size-vector->size) * vector->element_size;
        reserve(vector, new_size);
        void *dest = (char * )vector->data+vector->size*vector->element_size;
        memset(dest,0,diff_size);
    }
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    if(vector->size >= vector->capacity){
        //printf("Capacity: %d Size: %d\n", vector->capacity, vector->size);
        reserve(vector, vector->capacity*2);
    }
    void *dest = (char *)vector->data + vector->size*vector->element_size;
    memcpy(dest, value, vector->element_size);
    vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
    //vector->size = 0; <- potencjalna druga opcja
    free(vector->data);
    vector->size = 0;
    vector->data = malloc(vector->size*vector->element_size);
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    //printf("S: %d\n", vector->size);
    if (vector->size >= vector->capacity) {
        reserve(vector, vector->capacity * 2);
    }
    void *position = (char*)vector->data;
    if (index < vector->size) {
        memmove(position + vector->element_size * (index + 1), position + vector->element_size * index, vector->element_size * (vector->size - index));
    }
    memcpy(position + vector->element_size * index, value, vector->element_size);
    vector->size++;
}


// Erase element at position index
void erase(Vector *vector, size_t index) {
    void *src = (char *)vector->data + (index + 1) * vector->element_size;
    void *dest = (char *)vector->data + index * vector->element_size;
    size_t len = vector->size - index;
    //printf("%d\n",len);
    memmove(dest, src, len * vector->element_size);
    vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    char *data = (char*)vector->data;
    int pos = 0;
    while(pos < vector->size){
        if(!cmp(data+pos*vector->element_size, value)){
            erase(vector, pos);
        }
        else
            pos++;
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    char *data = (char*)vector->data;
    int pos = 0;
    while(pos < vector->size){
        if(predicate((void *)(data+pos*vector->element_size))){
            erase(vector, pos);
        }
        else
            pos++;
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    realloc(vector->data, vector->size*vector->element_size);
    vector->capacity = vector->size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    const int *a = (int*)v1;
    const int *b = (int*)v2;
    return *a-*b;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    const char *a = (char*)v1;
    const char *b = (char*)v2;
    return *a-*b;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    const struct Person *a = (const struct Person*)p1;
    const struct Person *b = (const struct Person*)p2;
    if(a->age == b->age){
        if(strcmp(a->first_name, b->first_name) == 0){
            return strcmp(a->last_name, b->last_name);
        }
        return strcmp(a->first_name, b->first_name);
    }
    return b->age - a->age;
}

// predicate: check if number is even
int is_even(void *value) {
    int *num = (int*)(value);
    return (*num+1)%2;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char *vowel = (char *)value;
    char VOWEL[12] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
    for (int i = 0; i < 12; ++i) {
        if (VOWEL[i] == *vowel) {
            return 1;
        }
    }
    return 0;
}


// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    const struct Person *a = (const struct Person*)person;
    if(a->age > 25 )
        return 1;
    return 0;
}

// print integer value
void print_int(const void *v) {
    const int *num = (const int*)v;
    printf("%d ",*num);
}

// print char value
void print_char(const void *v) {
    const char *let = (const char*)v;
    printf("%c ",*let);
}

// print structure Person
void print_person(const void *v) {
    const struct Person *person = (const struct Person*)v;
    printf("%d %s %s\n",person->age, person->first_name, person->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%d\n", vector->capacity);
    char *data = (char *)vector->data;
    for(int i =0; i<vector->size; ++i){
        print(data + i * vector->element_size);
    }
}

// read int value
void read_int(void* value) {
    scanf("%d",(int*)value);
}

// read char value
void read_char(void* value) {
    scanf(" %c", (char*)value);
}

// read struct Person
void read_person(void* value) {
    struct Person* person = (struct Person*)value;
    scanf("%d %s %s", &(person->age), person->first_name, person->last_name);
}

void *safe_malloc(size_t capacity){
    void *ptr = malloc(capacity);
    if(ptr == NULL){
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = safe_malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

