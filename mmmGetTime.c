#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /* for uint64  */
#include <time.h> /* for clock_gettime */
typedef int bool;
#define true 1
#define false 0
struct timespec { time_t tv_sec; /* seconds */ long tv_nsec;};

int clock_gettime(clockid_t clk_id, struct timespec *tp);
int clock_getres (clockid_t clk_id, struct timespec *res);

void printMatrix(double** x, int r, int c) {
    int i, j;

    for(i = 0; i < r; i ++) {
        for(j = 0; j < c; j++) {
            printf("%f ", x[i][j]);
        }
        printf("\n");
    }
}

double **allocateMatrix(int Rows, int Cols, bool init)
{
    double **matrix = (double **)malloc(Rows * sizeof(double *));
    int row;

    for (row = 0; row < Rows; row++) {
        matrix[row] = (double *)malloc(Cols * sizeof(double));
    }

    double count = 0;
    int i, j;

    for(i = 0; i < Rows; i ++) {
        for(j = 0; j < Cols; j++) {
            if (init) {

                matrix[i][j] = count;
            } else {
                matrix[i][j] = 0;
            }
            count++;
        }
    }

    return matrix;
}



double **matrixMultipyIJK(double**  x, double**  y, int r, int c) {
    int i, j, k;

    double**  z = allocateMatrix(r, c, false);

    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            for(k = 0; k < r; k++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}

double** matrixMultipyJIK(double** x, double** y, int r, int c) {
    int i, j, k;

    double**  z = allocateMatrix(r, c, false);

    for(j = 0; j < c; j++) {
        for(i = 0; i < r; i++) {
            for(k = 0; k < c; k++) {
                z[j][i] += x[j][k] * y[k][i];
            }
        }
    }

    return z;
}
double** matrixMultipyJKI(double** x, double** y, int r, int c) {

    double** z = allocateMatrix(r, c, false);
    int i, j, k;

    for(j = 0; j < c; j++) {
        for(k = 0; k < c; k++) {
            for(i = 0; i < r; i++) {
                z[j][i] += x[j][k] * y[k][i];
            }
        }
    }

    return z;
}
double** matrixMultipyKJI(double** x, double** y, int r, int c) {

    int i, j, k;

    double** z = allocateMatrix(r, c, false);

    for(k = 0; k < r; k++) {
        for(j = 0; j < c; j++) {
            for(i = 0; i < r; i++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}
double** matrixMultipyIKJ(double** x, double** y, int r, int c) {

    int i, j, k;

    double** z = allocateMatrix(r, c, false);

    for(i = 0; i < r; i++) {
        for(k = 0; k < r; k++) {
            for(j = 0; j < c; j++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}

double** matrixMultipyKIJ(double** x, double** y, int r, int c) {
    int i, j, k;

    double** z = allocateMatrix(r, c, false);

    for(k = 0; k < r; k++) {
        for(i = 0; i < r; i++) {
            for(j = 0; j < c; j++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}



int main(int argc, char** argv) {
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    struct timespec tick, tock;
    uint64_t execTime; /*time in nanoseconds */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    double** C = matrixMultipyIJK(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");


    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    C = matrixMultipyJIK(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    C = matrixMultipyKJI(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    C = matrixMultipyKIJ(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");



    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    C = matrixMultipyIKJ(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");



    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tick);
    C = matrixMultipyJKI(A, B, rows, cols);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tock);
    execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
    printf("\n\n");

    return 0;
}



