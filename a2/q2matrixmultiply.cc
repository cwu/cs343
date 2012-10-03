#include <uC++.h>

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
#include <cstdlib>

/**
 * This class acts as a task to multiply a row of a matrix.
 */
_Task RowMultiplier {
    private:
        int *xRow;
        int **Y;
        int *zRow;
        int xcyr;
        int ycols;

    /**
     * Calculates the row in the zRow based on the xRow and Y matrix.
     */
    void main() {
        for (int c = 0; c < ycols; c++) {
            zRow[c] = 0;
            for (int i = 0; i < xcyr; i++) {
                zRow[c] += xRow[i] * Y[i][c];
            }
        }
    }

    public:
        RowMultiplier(int *xRow, int **Y, int*zRow, int xcyr, int ycols) :
            xRow(xRow), Y(Y), zRow(zRow), xcyr(xcyr), ycols(ycols) {}
};

/**
 * prints program usage and exits as a failure.
 *
 * argv - the arguments fed into the program
 */
void usage(char **argv) {
    cerr << "Usage: " << argv[0] << " xrows xcols-yrows ycols "
         << " [ X-matrix-file Y-matrix-file ]" << endl;
    exit(EXIT_FAILURE);
}

/**
 * Performs a matrix multiplication (X * Y = Z)
 *
 * Z - output matrix
 * X - first input matrix
 * xr - number of rows of matrix X
 * xc - number of columns of matrix X and rows of matrix Y
 * Y - second input matrix
 * yc - number of columns of matrix Y
 */
void matrixmultiply( int *Z[ ], int *X[ ], int xr, int xc, int *Y[ ], int yc ) {
    RowMultiplier *multipliers[xr];
    for (int r = 0; r < xr; r++) {
        multipliers[r] = new RowMultiplier(X[r], Y, Z[r], xc, yc);
    }

    for (int r = 0; r < xr; r++) {
        delete multipliers[r];
    }
}

/**
 * Allocates a new matrix
 *
 * rows - number of rows the matrix should have
 * cols - number of columns the matrix should have
 *
 * returns the matrix
 */
int** newMatrix(int rows, int cols) {
    int ** matrix = new int*[rows];
    for (int r = 0; r < rows; r++) {
        matrix[r] = new int[cols];
    }
    return matrix;
}

/**
 * Frees a matrix
 *
 * rows - number of rows the matrix has
 */
void freeMatrix(int **matrix, int rows) {
    for (int r = 0; r < rows; r++) {
        delete [] matrix[r];
    }
    delete [] matrix;
}

/**
 * Generates a new matrix with 37 as all of its elements
 *
 * rows - number of rows the matrix should have
 * cols - number of columns the matrix should have
 *
 * returns the matrix
 */
int** generateMatrix(int rows, int cols) {
    int **matrix = newMatrix(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            matrix[r][c] = 37;
        }
    }
    return matrix;
}

/**
 * Reads a matrix from a file.
 *
 * matrixFile - the ifstream pointing to the file to read from
 * rows - number of rows the matrix should have
 * cols - number of columns the matrix should have
 *
 * returns the matrix
 */
int** readMatrix(ifstream &matrixFile, int rows, int cols) {
    int **matrix = newMatrix(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            matrixFile >> matrix[r][c];
        }
    }
    return matrix;
}

/**
 * Prints the multiplicaiton of two matricies (X*Y=Z).
 *
 * X - 1st input matrix
 * Y - 2nd input matrix
 * Z - output matrix
 * xrows - number of rows of X
 * xcols - number of columns of X and number of rows of Y
 * ycols - number of columns of Y
 */
void printMatricies(int *X[], int *Y[], int *Z[], int xrows, int xcols, int ycols) {
    const int cellSize = 8;

    // print the top half with the matrix Y
    int yrows = xcols;
    int marginY = (cellSize + 1) * xcols + 4;
    for (int r = 0; r < yrows; r++) {
        cout << setw(marginY) << "|";
        for (int c = 0; c < ycols; c++) {
            cout << " " << setw(cellSize) << Y[r][c];
        }
        cout << endl;
    }

    // print the divider
    for (int i = 0; i < marginY - 1; i++) {
        cout << '-';
    }
    cout << '*';
    for (int i = 0; i < (cellSize + 1) * ycols; i++) {
        cout << '-';
    }
    cout << endl;

    // print the bottom half with X | Z
    for (int r = 0; r < xrows; r++) {
        for (int c = 0; c < xcols; c++) {
            cout << setw(cellSize) << X[r][c] << " ";
        }
        cout << "   |";
        for (int c = 0; c < ycols; c++) {
            cout << " " << setw(cellSize) << Z[r][c];
        }
        cout << endl;
    }
}

void uMain::main() {
    if (argc < 4 || argc > 6 || argc == 5) {
        usage(argv);
    }

    // read in the matrix sizes
    int xrows = atoi(argv[1]);
    int xcols = atoi(argv[2]);
    int yrows = xcols;
    int ycols = atoi(argv[3]);

    if (xrows <= 0 || xcols <= 0 || ycols <= 0) {
        usage(argv);
    }

    int **X;
    int **Y;
    bool specifiedMatrixFiles = argc == 6;

    // If matrix files are specified then read from the files
    if (specifiedMatrixFiles) {
        try {
            ifstream matrixFile(argv[4]);
            X = readMatrix(matrixFile, xrows, xcols);
        } catch (uFile::Failure &e) {
            cerr << "Error! Could not open file \"" << argv[4] << "\"" << endl;
            usage(argv);
        }

        try {
            ifstream matrixFile(argv[5]);
            Y = readMatrix(matrixFile, yrows, ycols);
        } catch (uFile::Failure &e) {
            cerr << "Error! Could not open file \"" << argv[4] << "\"" << endl;
            usage(argv);
        }
    } else {
        X = generateMatrix(xrows, xcols);
        Y = generateMatrix(yrows, ycols);
    }

    // setup for parallize
    uProcessor p[xrows - 1] __attribute__((unused)); // number of CPUs

    int **Z = newMatrix(xrows, ycols);
    matrixmultiply(Z, X, xrows, xcols, Y, ycols);

    if (specifiedMatrixFiles) {
        printMatricies(X, Y, Z, xrows, xcols, ycols);
    }

    // cleanup
    freeMatrix(X, xrows);
    freeMatrix(Y, yrows);
    freeMatrix(Z, xrows);
}
