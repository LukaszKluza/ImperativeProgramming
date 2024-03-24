#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->size = size;
    p_table->ht = (ht_element*)malloc(sizeof (ht_element)*size);
    p_table->no_elements = 0;
    p_table->dump_data = dump_data;
    p_table->create_data = create_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
    for(int i =0; i<size; ++i){
        p_table->ht[i].next = NULL;
    }
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element *curr = p_table->ht[n].next;
    while(curr != NULL){
        p_table->dump_data(curr->data);
        curr = curr->next;
    }
    /*for(int i=0; i<p_table->size; ++i){
        printf("%d --> ", i);
        ht_element *curr = p_table->ht[i].next;
        while(curr != NULL){
            p_table->dump_data(curr->data);
            curr = curr->next;
        }
        printf("\n");
    }*/
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    free_data(to_delete->data);
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    for (size_t i = 0; i < p_table->size; i++) {
        ht_element *curr = p_table->ht[i].next;
        while (curr != NULL) {
            ht_element *next = curr->next;
            if (p_table->free_data != NULL)p_table->free_data(curr->data);
            free(curr);
            curr = next;
        }
    }
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table) {
    size_t old_size = p_table->size;
    size_t new_size = old_size*2;
    ht_element *new_ht = (ht_element *)malloc(sizeof(ht_element)*new_size);
    ht_element *old_ht = p_table->ht;
    p_table->size = new_size;
    p_table->no_elements = 0;
    p_table->ht = new_ht;
    for(int i =0; i<new_size; ++i){
        p_table->ht[i].next = NULL;
    }
    for(size_t i =0; i<old_size; ++i){
        ht_element *curr = old_ht[i].next;
        while(curr != NULL){
            ht_element *next = curr->next;
            size_t new_code = p_table->hash_function(curr->data, new_size);
            curr->next = p_table->ht[new_code].next;
            p_table->ht[new_code].next = curr;
            p_table->no_elements++;
            curr = next;
        }
    }
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
    size_t hash_code = p_table->hash_function(data, p_table->size);
    ht_element *curr = p_table->ht[hash_code].next;
    ht_element *prev = NULL;
    while(curr != NULL && p_table->compare_data(curr->data, data) != 0){
        prev = curr;
        curr = curr->next;
    }
    if(curr != NULL && p_table->compare_data(curr->data, data)==0){
        return prev;
    }
    return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    size_t hash_code = p_table->hash_function(*data, p_table->size);
    ht_element *curr = p_table->ht[hash_code].next;
    while(curr != NULL && p_table->compare_data(curr->data, *data) != 0){
        curr = curr->next;
    }
    if(curr != NULL && p_table->compare_data(curr->data, *data) == 0){
        return curr;
    }
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    //printf("%c %d\n", data->char_data, p_table->hash_function(*data, p_table->size));
    size_t hash_code = p_table->hash_function(*data, p_table->size);
    if(get_element(p_table, data) != NULL){
        return;
    }
    ht_element *new_element = (ht_element*)malloc(sizeof(ht_element));
    new_element->data = *data;
    new_element->next = NULL;
    if (p_table->ht[hash_code].next == NULL) {
        p_table->ht[hash_code].next = new_element;
    } else {
        new_element->next = p_table->ht[hash_code].next;
        p_table->ht[hash_code].next = new_element;
    }
    p_table->no_elements++;
    if(p_table->size * MAX_RATE < p_table->no_elements){
        rehash(p_table);
    }
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    size_t hash_code = p_table->hash_function(data, p_table->size);
    ht_element *prev = find_previous(p_table, data);
    ht_element *to_delete;
    if(prev == NULL){
        if(p_table->ht[hash_code].next == NULL)return;
        if(p_table->compare_data(p_table->ht[hash_code].next->data, data) != 0)return;
    }
    if (prev == NULL) {
        to_delete = p_table->ht[hash_code].next;
        if(to_delete->next != NULL){
            p_table->ht[hash_code].next = to_delete->next;
        } else{
            p_table->ht[hash_code].next = NULL;
        }
    } else if(prev != NULL) {
        to_delete = prev->next;
        prev->next = to_delete->next;
    }
    p_table->no_elements--;
    free(to_delete);
}


// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data);
}

int cmp_int(data_union a, data_union b) {
    int x = a.int_data;
    int y = b.int_data;
    return x-y;
}

data_union create_int(void* value) {
    data_union result;
    int x;
    scanf("%d", &x);
    result.int_data = x;

    if (value != NULL) {
        *(data_union*)value = result;
    }

    return result;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    int x = (int)a.char_data;
    int y = (int)b.char_data;
    return x-y;
}

data_union create_char(void* value) {
    data_union result;
    char x;
    scanf(" %c", &x);
    result.char_data = x;

    if (value != NULL) {
        *(data_union*)value = result;
    }

    return result;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
}

void free_word(data_union data) {
}

int cmp_word(data_union a, data_union b) {
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
}

data_union create_data_word(void *value) {
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n",table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

