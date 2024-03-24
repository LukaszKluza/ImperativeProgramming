#include <stdio.h>
#include <math.h>


#define SIZE 40
int T[SIZE];
int zam = 0;

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void print_vector_int(int x[], int n) {
    for(int i = 0; i < n; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for(int i=0; i<m; ++i){
        for(int j=0; j<n; ++j){
            double sum = 0;
            for(int k=0; k<p; ++k){
                sum+=A[i][k]*B[k][j];
                //printf("%f %f ",A[i][k],B[k][j]);
            }
            //printf(" %f \n",sum);
            AB[i][j] = sum;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for(int i=0; i<n; ++i){
        if(A[i][i] == 0) return NAN;
    }
    //printf("KLOKO");
    double res = 1;
    for(int i=0; i<n-1; ++i){
        for(int j=i+1; j<n; ++j){
            if(A[i][i] ==0)return 0;
            double x = A[j][i]/A[i][i];
            for(int k=i; k<n; ++k){
                A[j][k]-=A[i][k]*x;
            }
        }
        res*=A[i][i];
        //print_mat(A,n,n);
    }
    /*print_mat(A,n,n);
    printf("%f %f\n",A[n-1][n-1],res);*/
    res*=A[n-1][n-1];
    //printf("%f ",res);
    return res;
}

/*void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}*/

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

void replace(double A[][SIZE], int T[], int n, int k){
    double maxi = 0;
    int max_id=0;
    for(int i=k; i<n; ++i){
        int poz = T[i];
        //printf("%f %f\n",fabs(A[poz][k]), fabs(maxi));
        if(fabs(A[poz][k]) > fabs(maxi)){
            maxi = A[poz][k];
            max_id = i;
        }
    }
    //printf("%d\n",max_id);
    if(k!=max_id)zam++;
    int temp = T[k];
    T[k] = T[max_id];
    T[max_id] = temp;
}

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
    double res = 1;
    for(int i=0; i<n; ++i)T[i]=i;
    for(int i=0; i<n-1; ++i){
        replace(A,T,n,i);
        int poz = T[i];
        double val = A[poz][i];
        for(int j=i+1; j<n; ++j){
            int wiersz = T[j];
            if(val ==0)return 0;
            double r = A[wiersz][i]/val;
            for(int k=i; k<n; ++k){
                A[wiersz][k]-=A[poz][k]*r;
            }
            b[wiersz]-=b[poz]*r;
        }
        res*=A[T[i]][i];
    }
    res*=A[T[n-1]][n-1];
    if(zam%2 ==1)res*=-1;
    if(res == 0) return 0;
    double K[n][SIZE];
    //printf("\nK \n");
    for(int i=0; i<n; ++i){
        int poz = T[i];
        for(int j=0; j<n; ++j){
            K[i][j]=A[poz][j];
        }
        x[i]=b[poz];
    }
    //print_mat(K,n,n);
    //print_vector(x,n);
    for(int i=n-1; i>=0; --i){
        double  val = K[i][i];
        if(fabs(val)<eps)return 0;
        K[i][i]/=val;
        x[i]/=val;
        for(int j=i-1; j>=0; --j){
            x[j]-= x[i]*K[j][i];
            K[j][i] = 0;
        }
    }
    /*printf("\n");
    print_mat(K,n,n);
    print_vector(x,n);*/
    return res;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    for(int i=0; i<n; ++i)B[i][i]=1;
    double res = 1;
    for(int i=0; i<n; ++i)T[i]=i;
    for(int i=0; i<n-1; ++i){
        replace(A,T,n,i);
        int poz = T[i];
        double val = A[poz][i];
        //print_vector_int(T,n);
        for(int j=i+1; j<n; ++j){
            int wiersz = T[j];
            if(val ==0)return 0;
            double r = A[wiersz][i]/val;
            for(int k=0; k<n; ++k){
                A[wiersz][k]-=A[poz][k]*r;
                B[wiersz][k]-=B[poz][k]*r;
            }
        }
        res*=A[T[i]][i];
    }
    res*=A[T[n-1]][n-1];
    if(zam%2 ==1)res*=-1;
    if(res == 0) return 0;
    double K[n][SIZE];
    double P[n][SIZE];
    for(int i=0; i<n; ++i){
        int poz = T[i];
        for(int j=0; j<n; ++j){
            K[i][j]=A[poz][j];
            P[i][j]=B[poz][j];
        }
    }
    /*printf("\n");
    print_mat(A,n,n);
    printf("\n");
    print_mat(B,n,n);
    printf("\n");
    print_mat(K,n,n);
    printf("\n");
    print_mat(P,n,n);
    printf("\n");
    printf("---------------------");*/
    for(int i=n-1; i>=0; --i){
        double  val = K[i][i];
        if(fabs(val)<eps)return 0;
        K[i][i]/=val;
        for(int k=0; k<n; ++k)P[i][k]/=val;
        for(int j=i-1; j>=0; --j){
            for(int k=0; k<n; ++k){
                P[j][k]-=P[i][k]*K[j][i];
            }
            /*printf("\n");
            print_mat(K,n,n);
            printf("\n");
            print_mat(P,n,n);
            printf("\n");*/
            K[j][i] = 0;

        }
    }
    /*print_mat(P,n,n);
    printf("\n");*/
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            B[i][j]=P[i][j];
        }
    }
    return res;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

