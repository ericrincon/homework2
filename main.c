#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

typedef int bool;
#define true 1
#define false 0
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

    int i, j, k;
    long long counters[3];
    int PAPI_events[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA,
            PAPI_L2_DCM,
            PAPI_L2_DCA,
            PAPI_TOT_INS,
            PAPI_LD_INS,
            PAPI_FP_INS
    };
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    double** C = matrixMultipyIJK(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );


    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    C = matrixMultipyJIK(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    C = matrixMultipyKJI(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );


    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    C = matrixMultipyKIJ(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );


    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    C = matrixMultipyIKJ(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );


    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events, 3 );
    C = matrixMultipyJKI(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[3],
           (double)counters[3] / (double)counters[4],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("%lld Total instructions in %lld cycles\n",
           counters[5], counters[0] );
    printf("%lld Total load instructions in %lld cycles\n",
           counters[6], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[7], counters[0] );
    return 0;
}



