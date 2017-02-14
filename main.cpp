#include <iostream>
#include <papi.h>
#include <array>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>
#include "papi.h"
using namespace std;


vector<vector<double>> allocateMatrix(int nbRows, int nbColumns, bool init) {
    vector<vector<double>> matrix;
    matrix.resize(nbRows);

    int count = 1;

    for(int i = 0 ; i < nbRows ; ++i)
    {
        //Grow Columns by n
        matrix[i].resize(nbColumns);
    }


    for(int i = 0; i < nbRows; i ++) {
        for(int j = 0; j < nbColumns; j++) {
            if (init)
                matrix[i][j] = count;
            else
                matrix[i][j] = 0;
            count++;
        }
    }

    return matrix;
}


vector<vector<double>> matrixMultipyIJK(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int i = 0; i < xNbRows; i++) {
        for(int j = 0; j < yNbColumns; j++) {
            for(int k = 0; k < xNbRows; k++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}

vector<vector<double>> matrixMultipyJIK(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int j = 0; j < yNbColumns; j++) {
        for(int i = 0; i < xNbRows; i++) {
            for(int k = 0; k < yNbColumns; k++) {
                z[j][i] += x[j][k] * y[k][i];
            }
        }
    }

    return z;
}
vector<vector<double>> matrixMultipyJKI(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int j = 0; j < yNbColumns; j++) {
        for(int k = 0; k < yNbColumns; k++) {
            for(int i = 0; i < xNbRows; i++) {
                z[j][i] += x[j][k] * y[k][i];
            }
        }
    }

    return z;
}
vector<vector<double>> matrixMultipyKJI(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int k = 0; k < xNbRows; k++) {
        for(int j = 0; j < yNbColumns; j++) {
            for(int i = 0; i < xNbRows; i++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}
vector<vector<double>> matrixMultipyIKJ(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int i = 0; i < xNbRows; i++) {
        for(int k = 0; k < xNbRows; k++) {
            for(int j = 0; j < yNbColumns; j++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}

vector<vector<double>> matrixMultipyKIJ(vector<vector<double>> x, vector<vector<double>> y) {
    int xNbRows = x.size();
    int yNbColumns = y[0].size();

    vector<vector<double>> z = allocateMatrix(xNbRows, yNbColumns, false);

    for(int k = 0; k < xNbRows; k++) {
        for(int i = 0; i < xNbRows; i++) {
            for(int j = 0; j < yNbColumns; j++) {
                z[i][j] += x[i][k] * y[k][j];
            }
        }
    }

    return z;
}

void printMatrix(vector<vector<double>> x) {
    for(int i = 0; i < x.size(); i ++) {
        for(int j = 0; j < x[0].size(); j++) {
            cout <<  x[i][j] << " ";
        }
        cout << "\n";
    }
}
static void test_fail(char *file, int line, char *call, int retval);

int main() {
    /* Initialize the PAPI library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);

    if (retval != PAPI_VER_CURRENT && retval > 0) {
        fprintf(stderr,"PAPI library version mismatch!\n");
        exit(1);
    }

    if (retval < 0) {
        fprintf(stderr, “Initialization error!\n”);
        exit(1);
    }

    fprintf(stdout, “PAPI Version Number\n”);
    fprintf(stdout, “MAJOR:    %d\n”, PAPI_MAJOR(retval));
    fprintf(stdout, “MINOR:    %d\n”, PAPI_MINOR(retval));
    fprintf(stdout, “REVISION: %d\n”, PAPI_REVISION(retval));

    float real_time, proc_time, mflops;
    long long flpins;
    vector<vector<double>> A = allocateMatrix(10, 10, true);
    vector<vector<double>> B = allocateMatrix(10, 10, true);

    vector<vector<double>> C = matrixMultipyIJK(A, B);
    printMatrix(C);

    int retval, EventSet = PAPI_NULL;
    long_long values[1];

    /* Initialize the PAPI library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);

    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr, "PAPI library init error!\n");
        exit(1);
    }

/* Create the Event Set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        printf("not sure what to do..");
    C = matrixMultipyJIK(A, B);
    /* Collect the data into the variables passed in */
    if((retval=PAPI_flops( &real_time, &proc_time, &flpins, &mflops))<PAPI_OK)
        test_fail(__FILE__, __LINE__, "PAPI_flops", retval);

    printf("Real_time:\t%f\nProc_time:\t%f\nTotal flpins:\t%lld\nMFLOPS:\t\t%f\n",
           real_time, proc_time, flpins, mflops);
    printf("%s\tPASSED\n", __FILE__);
    PAPI_shutdown();
    printMatrix(C);

    C = matrixMultipyKJI(A, B);
    printMatrix(C);

    C = matrixMultipyKIJ(A, B);
    printMatrix(C);

    C = matrixMultipyIKJ(A, B);
    printMatrix(C);

    C = matrixMultipyJKI(A, B);
    printMatrix(C);

    return 0;
}



