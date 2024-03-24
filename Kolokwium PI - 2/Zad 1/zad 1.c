#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _matrix{
    int rows;
    int cols;
    double *data;
}Matrix;

Matrix * create_matrix(int a, int b){

}

Matrix *copy_matrix(const Matrix *src) {
    Matrix *result = create_matrix(src->rows, src->cols);
    memcpy(result->data, src->data, src->rows * src->cols * sizeof(double));
    return result;
}


Matrix *multiply(const Matrix *pm, double v) {
    Matrix *result = create_matrix(pm->rows, pm->cols);
    for (int i = 0; i < pm->cols * pm->rows; ++i) {
        result->data[i] = pm->data[i] * v;
    }
    return result;
}

void inplane_add_scalar(const Matrix *pm, double v) {
    for (int i = 0; i < pm->cols * pm->rows; ++i) {
        pm->data[i] = pm->data[i] + v;
    }
}

int swap_rows(Matrix *m, int r1, int r2){ //zakladam ze wiersze numerujemy od 0
    if (r1 >= m->rows || r2 >= m->rows) return 0;
    for (size_t i = 0; i < m->cols; ++i) {
        double temp = m->data[r1 * m->cols + i];
        m->data[r1 * m->cols + i] = m->data[r2 * m->cols + i];
        m->data[r2 * m->cols + i] = temp;
    }
    return 1;
}

Matrix *transpose(const Matrix *psrc) {
    Matrix *result = create_matrix(psrc->cols, psrc->rows);
    for (int i = 0; i < psrc->rows; ++i) {
        for (int j = 0; j < psrc->cols; ++j) {
            result->data[j * result->cols + i] = psrc->data[i * psrc->cols + j];
        }
    }
    return result;
}

int compar(const void *a, const void *b) {
    const Matrix *m1 = (const Matrix *)a;
    const Matrix *m2 = (const Matrix *)b;
    return m1->data[0] -m2->data[0];
}


int main(){

    Matrix *matrix;
    qsort(matrix->data, matrix->rows, sizeof(Matrix), compar);
}