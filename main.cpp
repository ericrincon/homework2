#include <iostream>
#include <array>
#include <vector>
#include <papi.h>
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



void handle_error (int retval)
{
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}
void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}
int begin_papi(int Event) {
  int EventSet = PAPI_NULL;
  int rv;
  /* Create the Event Set */
  if ((rv = PAPI_create_eventset(&EventSet)) != PAPI_OK)
    handle_error(rv);
  if ((rv = PAPI_add_event(EventSet, Event)) != PAPI_OK)
    handle_error(rv);
  /* Start counting events in the Event Set */
  if ((rv = PAPI_start(EventSet)) != PAPI_OK)
    handle_error(rv);
  return EventSet;
}
long_long end_papi(int EventSet) {
  long_long retval;
  int rv;

  /* get the values */
  if ((rv = PAPI_stop(EventSet, &retval)) != PAPI_OK)
    handle_error(rv);

  /* Remove all events in the eventset */
  if ((rv = PAPI_cleanup_eventset(EventSet)) != PAPI_OK)
    handle_error(rv);

  /* Free all memory and data structures, EventSet must be empty. */
  if ((rv = PAPI_destroy_eventset(&EventSet)) != PAPI_OK)
    handle_error(rv);

  return retval;
}

int main(int argc, char** argv) {init_papi();
    int PAPI_events[] = {
            PAPI_TOT_CYC,
            PAPI_L2_DCM,
            PAPI_L2_DCA
    };

    vector<vector<double>> A = allocateMatrix(10, 10, true);
    vector<vector<double>> B = allocateMatrix(10, 10, true);

    int EventSet = begin_papi(PAPI_TOT_INS);
    vector<vector<double>> C = matrixMultipyIJK(A, B);
    long_long r = end_papi(EventSet);
    std::cout << "Total instructions: " << r << "\n";

    printMatrix(C);

    C = matrixMultipyJIK(A, B);
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



