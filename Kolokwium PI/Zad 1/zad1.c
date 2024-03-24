#include <stdio.h>
#include <stdlib.h>

typedef struct _matrix{
    int rows;
    int cols;
    double**data;
}Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols){
    pmatrix->rows = rows;
    pmatrix->cols = cols;
    pmatrix->data = (double**)malloc(sizeof(double*) * rows);
    for(int i =0; i<rows; ++i){
        pmatrix->data[i] = (double *) malloc(sizeof(double) *cols );
    }
    for(int i =0; i<rows; ++i){
        for(int j =0; j<cols; ++j){
            pmatrix->data[i][j] = i+j*(i+1);
        }
    }
    return 1;
}

int get(Matrix*pm, int row, int col, double *pvalue){
    if(row >= pm->rows || col>= pm->cols || !pm->data || col < 0 || row < 0){
        return 0;
    }
    *pvalue = pm->data[row][col];
    return 1;
}

int set(Matrix*pm, int row, int col, double pvalue){
    if(row >= pm->rows || col>= pm->cols || !pm->data || col < 0 || row < 0){
        return 0;
    }
    pm->data[row][col] = pvalue;
    return 1;
}

void create_identity_matrix(Matrix *pm, int size){
    pm->cols = pm->rows = size;
    pm->data = (double **) malloc(sizeof(double*)*size);
    for(int i =0; i<size; ++i){
        pm->data[i] = (double *) malloc(sizeof(double) *size);
    }
    for(int i =0; i<size; ++i){
        for(int j =0; j<size; ++j){
            pm->data[i][j]=0;
        }
        pm->data[i][i] = 1;
    }
}
double dot_prod_vect(const double *v1, const double *v2,int size){
    double res = 0;
    for(int i =0; i<size; ++i){
        res+=v1[i]*v2[i];
    }
    return res;
}

void mul_vet(double *result, const Matrix *pm, const double *vect){
    for(int i=0; i<pm->rows; ++i){
        result[i] = dot_prod_vect(pm->data[i], vect, pm->cols);
    }
}

void max_element_in_rows(double *maxdata,const Matrix *pm){
    double temp_max = -1e9-9;
    for(int i=0; i<pm->rows; ++i){
        temp_max = -1e9-9;
        for(int j=0; j<pm->cols; ++j){
            if(pm->data[i][j] > temp_max){
                temp_max = pm->data[i][j];
            }
        }
        maxdata[i] = temp_max;
    }
}

void dump_matrix(Matrix *pm){
    for(int i = 0; i<pm->rows; ++i){
        for(int j =0; j<pm->cols; ++j){
            printf("%.2f ", pm->data[i][j]);
        }
        printf("\n");
    }
}

void print_vector(const double* vector, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%.2f ", vector[i]);
    }
    printf("\n");
}

int main(){
    Matrix identity;
    create_matrix(&identity, 4,4);
    double vector[4] = { 0, 2.0, 3.0 };
    double result[4];
    mul_vet(result,&identity,vector);
    dump_matrix(&identity);
    printf("------------\n");
    print_vector(result,4);
    printf("------------\n");
    max_element_in_rows(result, &identity);
    print_vector(result,4);
    return 0;
}