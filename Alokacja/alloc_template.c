#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[cols*row+col];
}

void set(int cols, int row, int col, int *A, int value) {
    A[cols*row+col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    long res = 0;
    for(int i =0; i < rowsA; ++i ){
        for(int j=0; j < colsB; ++j){
            res = 0;
            for(int k=0; k < colsA; ++k){
                res += get(colsA,j,k,A)* get(colsB,k,i,B);
            }
            set(colsB,j,i,AB,res);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for(int i=0; i<rows; ++i){
        for(int j=0; j<cols; ++j){
            scanf("%d",&t[i*cols+j]);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i=0; i<rows; ++i){
        for(int j=0; j<cols; ++j){
            printf("%d ",t[i*cols+j]);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    int lines = 0;
    char line[BUF_SIZE];
    while (fgets(line, BUF_SIZE, stdin)) {
        if (line[0] == EOF) {
            return lines;
        }
        array[lines] = (char*) malloc(strlen(line) + 1);
        strcpy(array[lines], line);
        lines++;
    }
    return lines;
}

void write_char_line(char *array[], int n) {
    printf("%s",array[n]);
}

void delete_lines(char *array[]) {
    size_t n = strlen(array);
    for(int i=0; i<n; ++i){
        free(array[i]);
    }
}

void write_int_line_cont(int *ptr_array[], int n) {
    int* line = ptr_array[n-1];
    int col_count = line[0];
    for(int i=1; i<=col_count; ++i){
        printf("%d ",line[i]);
    }
    printf("\n");
}

int read_int_lines_cont(int *ptr_array[]) {
    int line_count = 0,col_count = 1,value=0, flag = 1;
    char ch;
    ptr_array[0] = (int*)malloc(TAB_SIZE * sizeof(int));
    while (ch = getchar()){
        if (ch == EOF){
            return line_count;
        } else if (ch == '\n'){
            ptr_array[line_count][col_count] = value*flag;
            value = 0;
            flag = 1;
            ptr_array[line_count][0] = col_count;
            line_count+=1;
            col_count = 1;
            ptr_array[line_count] = (int*)malloc(TAB_SIZE * sizeof(int));
        } else if(ch == ' '){
            ptr_array[line_count][col_count] = value*flag;
            col_count++;
            value = 0;
            flag = 1;
        } else if(ch == '-'){
            flag = -1;
        }else{
            value = value * 10 + (ch - '0');
        }
    }
    return line_count+1;
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int cmp (const void *a, const void *b) {
    line_type *line1 = (line_type *)a;
    line_type *line2 = (line_type *)b;
    return line1->average > line2->average;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array,line_count, sizeof(line_type), cmp);
}

int read_int_lines(line_type lines_array[]) {
    int line_count = 0, len = 0, sum = 0, value = 0, flag = 1;
    char ch;
    lines_array[0].values = (int*)malloc(TAB_SIZE * sizeof(int));
    int temp_line[BUF_SIZE];
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') {
            temp_line[len++] = value * flag;
            sum += value * flag;
            lines_array[line_count].values = (int*)malloc(len * sizeof(int));
            memcpy(lines_array[line_count].values, temp_line, len * sizeof(int));
            lines_array[line_count].len = len;
            lines_array[line_count].average = (double)(sum / len);
            value = 0;
            len = 0;
            sum = 0;
            flag = 1;
            ++line_count;
            lines_array[line_count].values = (int*)malloc(TAB_SIZE * sizeof(int));
        } else if (ch == ' ') {
            temp_line[len++] = value * flag;
            sum += value*flag;
            value = 0;
            flag = 1;
        } else if (ch == '-') {
            flag = -1;
        } else {
            value = value * 10 + (ch - '0');
        }
    }
    return line_count;
}


void write_int_line(line_type lines_array[], int n) {
    for (int i = 0; i < lines_array[n].len; ++i) {
        printf("%d ", lines_array[n].values[i]);
    }
    printf("\n%.2f",lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
    for(int i =0; i<line_count; ++i){
        free(array[i].values);
    }
}

typedef struct {
    int r, c, v;
} triplet;

int cmp_triplets(const void *t1, const void *t2) {
    triplet *a = (triplet *)t1;
    triplet *b = (triplet *)t2;
    if(a->r == b->r){
        return a->c > b->c;
    }
    return a->r > b->r;
}

int read_sparse(triplet *triplet_array, int n_triplets) {
    for(int i =0; i<n_triplets; ++i){
        scanf("%d %d %d", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }
    return n_triplets;
}


void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet_array[0]), cmp_triplets);
    for(int i =0; i<n_triplets; ++i){
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
    }
    int temp_r = 1;
    for(int i=1; i<=n_triplets; ++i){
        if(triplet_array[i-1].r != triplet_array[i].r){
            R[triplet_array[i-1].r+1] = temp_r;
        }
        temp_r +=1;
    }
    for(int j=1; j<=rows; ++j){
        if(R[j] == 0)R[j] = R[j-1];
    }

}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; ++i) {
        int y_i = 0;
        for (int j = R[i]; j < R[i+1]; ++j) {
            y_i = y_i + V[j] * x[C[j]];
        }
        y[i] = y_i;
    }
}

void read_vector(int *v, int n) {
    for(int i=0; i<n; ++i){
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i=0; i<n; ++i){
        printf("%d ",v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int(); // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

