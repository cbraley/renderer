#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <iostream>
//--
#include "Assert.h"

/**
 *  Utility functions for dealing with matrices.
 *  @uathor Colin Braley
 */
namespace MatrixUtils{

template<typename T>
inline bool isCloseTo(T a, T b, T eps){
    return abs(double(a-b)) <= double(eps);
}

/**
 *  Check for closeness.
 */
template<int ROWS, int COLS, typename T>
bool equalityWithinEpsilon( T** a, T** b, T eps){
//TODO: This is a hack to woraround not having a function
//std::abs<T>...fix this hack macros are evil
#define ABS_MACRO(x) ((x>-x?x:-x))
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++){
            const T diff = (a[r][c] - b[r][c]);
            if( ABS_MACRO(diff) > eps){
                return false;
            }
        }
    }
#undef ABS_MACRO
    return true;
}


/**
 *  Allocate an uninitialized matrix.
 */
template<int ROWS, int COLS, typename T>
T** allocMatrix(){
    T** mat = new T*[ROWS];
    for(int i = 0; i < ROWS; i++){
        mat[i] = new T[COLS];
    }
    return mat;
}


/**
 *  Cleanup memory for a matrix.
 */
template<int ROWS, int COLS, typename T>
void freeMatrix(T** mat){
    for(int i = 0; i < ROWS; i++){
        delete[] mat[i];
        mat[i] = NULL;
    }
    delete[] mat;
    mat = NULL;
}

template<int ROWS, int COLS, typename T>
void fillMatrix(T** mat, const T c){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            mat[i][j] = c;
        }
    }
}


template<int ROWS, int COLS, typename T>
void setMatrixData(T** mat, const T data[ROWS][COLS] ){
    Assert(mat != NULL);
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            mat[i][j] = data[i][j];
        }
    }
}



/**
 *  Print a matrix to an ostream.
 */
template<int ROWS, int COLS, typename T>
std::ostream& printMat(std::ostream& os, T** mat){
    Assert(mat != NULL);
    COMPILE_TIME_ASSERT( ROWS > 0 && COLS > 0 );

    os << "Matrix with dimensions: " << ROWS << " x " << COLS << std::endl;
    for(int r = 0; r < ROWS; r++){
        os << "[ ";
        for(int c = 0; c < COLS; c++){
           os << mat[r][c] << " ";
        }
        os << "]" << std::endl;
    }
    os << std::endl;
    return os;
}


template<int ROWS, int COLS, typename T>
bool rowSwap(T** mat, int row1, int row2){
    COMPILE_TIME_ASSERT( ROWS > 0 && COLS > 0 );
    Assert(mat != NULL);
    Assert(row1 >= 0 && row1 < ROWS);
    Assert(row2 >= 0 && row2 < ROWS);
    //--
    if(row1 != row2){
        //All we need to do is swap pointers
        T* tmpPtr = mat[row1];
        mat[row1] = mat[row2];
        mat[row2] = tmpPtr;
        return true;
    }else{ //Cant swap a row with itself
        return false;
    }
}


template<int ROWS, int COLS, typename T>
bool colSwap(T** mat, int col1, int col2){
    COMPILE_TIME_ASSERT( ROWS > 0 && COLS > 0 );
    Assert(mat != NULL);
    Assert(col1 >= 0 && col1 < COLS);
    Assert(col2 >= 0 && col2 < COLS);
    //--
    if(col1 != col2){
        for(int r = 0; r < ROWS; r++){
            const T temp = mat[r][col1];
            mat[r][col1] = mat[r][col2];
            mat[r][col2] = temp;
        }
        return true;
    }else{
        return false;
    }
}


//TODO: Use scaled partial pivoting
/**
 *  Perform in place Gaussian elimination.
 */
template<int ROWS, int COLS, typename T>
void gaussElim(T** mat){

    COMPILE_TIME_ASSERT( ROWS > 0 && COLS > 0 );
    Assert(mat != NULL);

    int c = 0;
    for(int r = 1; r < ROWS; r++){

        //std::cout << "Gauss elim step " << r << std::endl;
        //printMat<ROWS,COLS,T>(std::cout, mat);

        const T firstThisRow = mat[r][c];

        //If we are at a 0, we may need to do a swao
        const T TOL = T(0.0001);
        if(isCloseTo<T>(firstThisRow, T(0.0), TOL)){ //TODO: Make this a better comparison
            bool madeSwap = false;
            for(int i = r+1; i < ROWS; i++){
                if(mat[i][c] != T(0.0)){
                    rowSwap<ROWS,COLS,T>(mat, i, r);
                    madeSwap = true;
                    break;
                }
            }
            //Uh-oh didn't find a good choice!   Matrix is singular!
            if(!madeSwap){
                std::cerr << "Matrix: " << std::endl;
                printMat<ROWS,COLS,T>(std::cerr, mat);
                std::cerr << " is singular!  Row " << r << " has an issue." << std::endl;
                std::cerr << "Element at index (" << r << ", " << c << ") was " << firstThisRow << "." << std::endl;

            }
            Assert(madeSwap);
            //TODO: Throw
        }

        const T firstPrevRow = mat[r-1][c];
        //x * firstPrevRow + firstThisRow = 0
        const T x = -firstThisRow / firstPrevRow;

        //Update row r
        for(int cit = 0; cit < COLS; cit++){
            mat[r][cit] += x * mat[r-1][cit];
        }
        ++c;
    }
}



/**
 *  Perform in place Gaussian-Jordan elimination.
 */
template<int ROWS, int COLS, typename T>
void gaussJordanElim(T** mat){

    COMPILE_TIME_ASSERT( ROWS > 0 && COLS > 0 );
    Assert(mat != NULL);

    //First do Gaussian elimination
    gaussElim<ROWS,COLS,T>(mat);
    //...the matrix is now upper triangular
    //(could solve by backsubsitution at this point if we were doing Ax=b)
    //Now we need to do the "Jordan part" to reach reduced echelon form

    //Try to reach reduced echelon form
    int c = ROWS-1;
    for(int r = ROWS-1; r > 0; r--){
        const T x = -mat[r-1][c] / mat[r][c];
        //x*me + above = 0
        for(int ct = 0; ct < COLS; ct++){
            mat[r-1][ct] += x * mat[r][ct];
        }
        --c;
    }

    //Now finally, we need to divide by pivot elements
    for(int i = 0; i < ROWS; i++){
        const T piv = mat[i][i];
        Assert(piv != T(0.0)); //Singular!
        const T pivInv = T(1.0) / piv;
        for(int j = i; j < COLS; j++){
            mat[i][j] *= pivInv;
        }
    }
}



/**
 *  Invert a N by N matrix using Gauss-Jordan elimination.
 *  Only should be used for small matrices.
 *
 *  Returns a new NxN array, the callee is responsible for freeing this
 *  memory.
 */
template<int N, typename T>
T** gaussJordanInvert(T** mat){

    //Verify at compile time that N is > 0
    //For various reasons we don't want to use unsigned type for N
    COMPILE_TIME_ASSERT(N > 1);
    Assert(mat != NULL);


    //Form the augmented matrix
    //with dimensions N rows by 2N columns
    const int AUG_ROWS = N;
    const int AUG_COLS = N * 2;
    //allocate space
    T** augMat = new T*[AUG_ROWS];
    for(int i = 0; i < AUG_ROWS; i++){
        augMat[i] = new T[AUG_COLS];
    }
    //and copy in the data
    for(int r = 0; r < AUG_ROWS; r++){
        for(int c = 0; c < AUG_COLS; c++){
            if(c < N){ //Normal section
                augMat[r][c] = mat[r][c];
            }else{ //Augment with I
                augMat[r][c] = (r == (c-N)) ? 1.0f : 0.0f;
            }
        }
    }

    //Augmented matrix is now formed
    //...now perform gauss Jordan elimination
//    std::cout << "Augmented matrix PRE elim: " << std::endl;
 //   printMat<AUG_ROWS,AUG_COLS,T>(std::cout, augMat);
    gaussJordanElim<AUG_ROWS, AUG_COLS, T>(augMat);
 //   std::cout << "Augmented matrix post elim: " << std::endl;
  //  printMat<AUG_ROWS,AUG_COLS,T>(std::cout, augMat);


    //Take out the inverse matrix
    T** ret = new T*[N];
    for(int i = 0; i < N; i++){
        ret[i] = new T[N];
    }
    for(int r = 0; r < N; r++){
        for(int c = 0; c < N; c++){
            ret[r][c] = augMat[r][c+N];
        }
    }

    //Free augmented matrix
    for(int i = 0; i < N; i++){
        delete[] augMat[i];
        augMat[i] = NULL;
    }
    delete[] augMat; augMat = NULL;

    //All done
    return ret;
}

//Specialization of GJ Inverse finding for 2x2 matrices...
//Their inverse can be found faster using analytical determinant formula
template<>
float** gaussJordanInvert<2,float>(float** mat){
    Assert(mat != NULL);

    //Allocate memory
    float** ret = new float*[2];
    ret[0] = new float[2];
    ret[1] = new float[2];

    //Apply formula
    const float a = mat[0][0];
    const float b = mat[0][1];
    const float c = mat[1][0];
    const float d = mat[1][1];
    const float scalar = float(1.0) / ((a*d)-(b*c));

    ret[0][0] = d  * scalar;
    ret[0][1] = -b * scalar;
    ret[1][0] = -c * scalar;
    ret[1][1] = a  * scalar;

    return ret;
}

}


#endif //MATRIX_UTILS_H
