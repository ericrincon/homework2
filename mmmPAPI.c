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



void ijk(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyIJK(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyIJK(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyIJK(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyIJK(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}
void jik(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyJIK(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyJIK(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyJIK(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyJIK(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}
void jki(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyJKI(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyJKI(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyJKI(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyJKI(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}
void kji(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyKJI(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyKJI(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyKJI(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyKJI(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}
void IKJ(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyIKJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyIKJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyIKJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyIKJ(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}
void KIJ(int rows, int cols) {

    double** A = allocateMatrix(rows, cols, true);
    double** B = allocateMatrix(rows, cols, true);

    int i, j, k;
    long long counters[3];
    int PAPI_events_L1[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };


    int PAPI_events_L2[] = {
            PAPI_TOT_CYC,
            PAPI_L1_DCM,
            PAPI_L1_DCA
    };

    int PAPI_events_other[] = {
            PAPI_TOT_CYC,
            PAPI_LD_INS,
            PAPI_FP_INS
    };

    int PAPI_events_ins[] = {
            PAPI_TOT_CYC,
            PAPI_TOT_INS
    };



    /**L1 Cache Misses**/
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L1, 3);

    double** C = matrixMultipyKIJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3 );
    printf("%lld L1 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    printf("%lld Total cycles in %lld cycles\n",
           counters[0], counters[0] );
    printf("\n\n");
    
    
    /**L1 Cache Misses End**/


    /**L2 Cache Misses**/

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_L2, 3);

    C = matrixMultipyKIJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n",
           counters[1],
           (double)counters[1] / (double)counters[2],
           counters[0] );
    /**L2 Cache Misses End**/



    /**INS  **/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);
    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_ins, 3);

    C = matrixMultipyKIJ(A, B, rows, cols);

    PAPI_read_counters( counters, 3);

    printf("%lld Total instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("\n\n");
    
    
    /**INS End**/




    /**Other Start**/
    A = allocateMatrix(rows, cols, true);
    B = allocateMatrix(rows, cols, true);

    PAPI_library_init(PAPI_VER_CURRENT);
    i = PAPI_start_counters( PAPI_events_other, 3);
    C = matrixMultipyKIJ(A, B, rows, cols);
    PAPI_read_counters( counters, 3 );

    printf("%lld Total load instructions in %lld cycles\n",
           counters[1], counters[0] );
    printf("%lld Total floating point instructions  in %lld cycles\n",
           counters[2], counters[0] );
    printf("\n\n");
    
    

    /**Other End**/



}




int main(int argc, char** argv) {
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    printf("Start IJK\n");
    ijk(rows, cols);
    printf("END IJK\n\n");

    printf("Start JIK\n");
    jik(rows, cols);
    printf("END JIK\n\n");

    printf("Start jki\n");
    jki(rows, cols);
    printf("END jki\n\n");

    printf("Start kji\n");
    kji(rows, cols);
    printf("END kji\n\n");


    printf("Start IKJ\n");
    IKJ(rows, cols);
    printf("END IKJ\n\n");

    printf("Start KIJ\n");
    KIJ(rows, cols);
    printf("END KIJ\n\n");

    return 0;
}



