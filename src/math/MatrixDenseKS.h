#ifndef MATH_MATRIX_DENSE_KS
#define MATH_MATRIX_DENSE_KS

#include <iostream>
#include <iomanip>
#include <cmath>
//--
#include "Assert.h"

//TODO: This is a hack!  Make this work for other types!
#define DFLT_EPSILON_VAL (T(0.00001))
template <typename T>
bool isClose(const T a, const T b, const T epsilon){
    return fabs(double(a) - double(b)) < epsilon;
}

template<typename T>
T absVal(const T v){
    return (T)abs((double)v);
}



/**
 *  Dense known size column vector.
 */
template <typename T, int NROWS>
class VectorDenseKS{
public:
    VectorDenseKS(){}
    VectorDenseKS(const T inData[NROWS]){
        COMPILE_TIME_ASSERT(NROWS > 0);
        for(int i = 0; i < NROWS; i++){
            data[i] = inData[i];
        }
    }
    inline void set(const int idx, const T val){
        Assert(idx >= 0 && idx < NROWS);
        data[idx] = idx;
    }
    inline T operator()(const int idx)const{
        Assert(idx >= 0 && idx < NROWS);
        return data[idx];
    }
    inline int getNumRows()const{ return NROWS; }

    friend std::ostream& operator<<(std::ostream& os, const VectorDenseKS<T,NROWS>& m){
        const int PWIDTH = 10;//TODO: REFACTOR THIS TO BE COMMON
        os << "Dense vector with " << m.getNumRows() << " rows: " << std::endl;
        os << '[';
        for(int j = 0; j < m.getNumRows(); j++){
            os << std::fixed << std::setw(PWIDTH) << (m.data[j]) << (j != m.getNumRows()-1 ? " " : "]^T");
        }
        os << std::endl;
        return os;
    }

private:
    T data[NROWS];
};

/**
 *  Class for dense, small, known-size(at compile time) matrices.  KS stands for "known-size."
 *  Templated on type for easy use with doubles, floats, bignums, whatever.  The template type T
 *  must be convertable via a c-tor taking a single double.  For example:
 *  T(0.0) and T(1.0) must return a valid T.
 *
 *  The primary benefit to fixed size matrices is that they do not need dynamic allocation and that
 *  loops can be unrolled.
 *
 *  The primary place this class is used is for 4x4 T=float transformation matrices.
 */
template <typename T, int NROWS, int NCOLS>
class MatrixDenseKS{
public:

    /// \brief Default c-tor creates identity matrix.
    MatrixDenseKS(){
        COMPILE_TIME_ASSERT(NROWS > 0 && NCOLS > 0);
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                mat[i][j] = (i == j ? T(1.0) : T(0.0));
            }
        }
    }
    // \brief Construct a matrix with a single same value everywhere.
    explicit MatrixDenseKS(const T val){
        COMPILE_TIME_ASSERT(NROWS > 0 && NCOLS > 0);
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                mat[i][j] = val;
            }
        }
    }
    MatrixDenseKS(const T** data){
        COMPILE_TIME_ASSERT(NROWS > 0 && NCOLS > 0);
        Assert(data != NULL);
        for(int i = 0; i < NROWS; i++){
            Assert(data[i] != NULL);
            for(int j = 0; j < NCOLS; j++){
                mat[i][j] = data[i][j];
            }
        }
    }
    MatrixDenseKS(const T data[NROWS][NCOLS]){
        COMPILE_TIME_ASSERT(NROWS > 0 && NCOLS > 0);
        for(int r = 0; r < NROWS; r++){
            for(int c = 0; c < NCOLS; c++){
                mat[r][c] = data[r][c];
            }
        }
    }

    /// \brief Copy constructor.
    MatrixDenseKS(const MatrixDenseKS& other){
        helpCopy(other);
    }
    /// \brief Assignment operator.
    MatrixDenseKS& operator=(const MatrixDenseKS& rhs){
        helpCopy(rhs);
        return *this;
    }


    /// \brief Get an individual element.  0 based indices used.
    inline T operator()(int r, int c)const{
        Assert(r >= 0 && c >= 0 && r < NROWS && c < NCOLS);
        return mat[r][c];
    }
    /// \brief Set a specific element.
    inline void set(int r, int c, const T val){
        Assert(r >= 0 && c >= 0 && r < NROWS && c < NCOLS);
        mat[r][c] = val;
    }


    /// \brief Get number of columns.
    inline int numCols()const{ return NCOLS; }
    /// \brief Get number of rows.
    inline int numRows()const{ return NROWS; }

    /// \brief Matrix-scalar multipication.
    MatrixDenseKS<T,NROWS,NCOLS> operator*(const T s)const{
        MatrixDenseKS<T,NROWS,NCOLS> ret(*this);
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                ret.mat[i][j] *= s;
            }
        }
        return ret;
    }
    //TODO: Include *= scalar operator
    //TODO: Include /= scalar operator
    //TODO: Include / scalar operator

    /// \brief Matrix-Matrix addition.
    MatrixDenseKS<T,NROWS,NCOLS> operator+(const MatrixDenseKS<T,NROWS,NCOLS>& rhs)const{
        MatrixDenseKS<T,NROWS,NCOLS> ret(*this);
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                ret.mat[i][j] += rhs.mat[i][j];
            }
        }
        return ret;
    }
    /// \brief Matrix-Matrix subtraction.
    MatrixDenseKS<T,NROWS,NCOLS> operator-(const MatrixDenseKS<T,NROWS,NCOLS>& rhs)const{
        MatrixDenseKS<T,NROWS,NCOLS> ret(*this);
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                ret.mat[i][j] -= rhs.mat[i][j];
            }
        }
        return ret;
    }
    //TODO: Include += Matrix operator
    //TODO: Include -= Matrix operator

    /// \brief Swap two rows; return true if a swap took place.
    inline bool swapRows(int r1, int r2){
        Assert(r1 >= 0 && r1 < NROWS && r2 >= 0 && r2 < NROWS);
        if(r1 != r2){
            for(int j = 0; j < NCOLS; j++){
                const T temp = mat[r1][j];
                mat[r1][j] = mat[r2][j];
                mat[r2][j] = temp;
            }
            return true;
        }else{
            return false;
        }
    }

    /// \brief Swap two columns; return true if a swap took place.
    inline bool swapCols(int c1, int c2){
        Assert(c1 >= 0 && c1 < NCOLS && c2 >= 0 && c2 < NCOLS);
        if(c1 != c2){
            for(int i = 0; i < NROWS; i++){
                const T temp = mat[i][c1];
                mat[i][c1] = mat[i][c2];
                mat[i][c2] = temp;
            }
            return true;
        }else{
            return false;
        }
    }

    /// \brief Row * scalar
    inline void rowMult(int r, const T s){
        Assert(r >= 0 && r < NROWS);
        for(int c = 0; c < NCOLS; c++){
            mat[r][c] *= s;
        }
    }

    /// \brief Let rRHS = r1 - (s*r2)
    inline void rowSub(int r1, int r2, const T s = T(1.0)){
        Assert(r1 >= 0 && r1 < NROWS && r2 >= 0 && r2 < NROWS);
        for(int c = 0; c < NCOLS; c++){
            mat[r1][c] -= (mat[r2][c] * s);
        }
    }


    //TODO: Implement SCALED partial pivoting.  This is more numerically stable.
    /**
     *  Gaussian elimination with partial pivoting.  Better than naive Gaussian Elimination because, when you
     *  run into a zero-value you don't get a divide by 0 issue.  Instead, the best pivot element in a given column
     *  is chosen.  This is all implemented without any explicit row swaps(an index vector is used instead).
     *
     *  Any row swaps necessary are performed at completion, but only if reorderOnCompletion is true(it is by default).
     */
    bool gaussElimPartialPiv(const bool reorderOnCompletion = true){
        int indexVec[NROWS]; //Set to 0,1,2,...,NROWS inside helpGaussElimPartialPiv
        const bool nonSingular = helpGaussElimPartialPiv(indexVec, reorderOnCompletion);
        return nonSingular;
    }

    /// \brief Compute Naive Gaussian Elimination on a given matrix.
    /// Returns true if successful; false if the matrix we couldn't complete the operations
    /// due to a bad pivtot.
    ///
    /// THIS METHOD SHOULD NORMALLY NOT BE USED.  USE Gauss-Elim with SCALED PARTIAL PIVOTING
    /// UNLESS YOU HAVE A GOOD REASON.
    bool gaussElimNaive(){
        int c = 0;
        for(int r = 0; r < NROWS-1; r++){
            std::cout << *this << std::endl;
            //Check for 0 element; we can't continue in this case.  Pivoting avoids this problem.
            const T elem = mat[r][c];
            if(isClose<T>(elem, T(0.0), DFLT_EPSILON_VAL)){ return false; }
            const T invElem  = T(1.0) / elem;
            for(int rr = r + 1; rr < NROWS; rr++){
                //Let b = this elem, we want to make it zero, Let a = the elem in the row above...
                //    b - m * a = 0
                //    m = b/a
                const T m = mat[rr][c] * invElem;
                for(int cc = 0;  cc < NCOLS; cc++){
                    mat[rr][cc] = mat[rr][cc] - (m * mat[r][cc]);
                }
            }
            ++c;
        }
        return true;
    }

    VectorDenseKS<T,NROWS> solveLinearSystem(const VectorDenseKS<T,NROWS>& rhsVal, bool& solveOK)const{

        VectorDenseKS<T,NROWS> rhs(rhsVal);
        MatrixDenseKS<T,NROWS,NCOLS> cpy(*this);
        int indexVec[NROWS];
        //TODO: Make this more effecient by not using the indexing vector after return
        //and not requiring any memory movement
        const bool nonSingular = cpy.helpGaussElimPartialPiv(indexVec, true);
        std::cout << "index vector " << std::endl;
        for(int i = 0; i < NROWS; i++){
            std::cout << "\t" << indexVec[i] << std::endl;
        }
        std::cout << "\t A matrix after gjeppiv " << cpy << std::endl;
        std::cout << "\t rhs vector before reodering " << rhs << std::endl;

        //Backsubstitution...if we had a singular matrix just skip it
        T res[NROWS];
        if(nonSingular)
            //Re-order the vector and zero out res
            for(int i = 0; i < NROWS; i++){
                res[i] = T(0.0);

                const int idxMoveTo = indexVec[i];
                if(idxMoveTo == i){
                    continue;
                }
                const T temp = rhs(i);
                rhs.set(i, rhs(idxMoveTo));
                rhs.set(idxMoveTo, temp);

                indexVec[i] = i;
                indexVec[idxMoveTo] = idxMoveTo;
            }
            std::cout << "\t rhs vector after reodering " << rhs << std::endl;

            //Perform backsubstitution without considering index vectors
            for(int i = NROWS-1; i >= 0; i--){
                T accum = T(0.0);
                for(int j = i+1; j < NCOLS; j++){
                    accum += cpy.mat[i][j] * res[j];
                }
                res[i] = (rhs(i) - accum) / cpy.mat[i][i];
            }

        solveOK = nonSingular;
        return VectorDenseKS<T,NROWS>(res);
    }

    MatrixDenseKS<T,NROWS,NCOLS> inverse(bool& completedOK)const{
        //Make augmented matrix by concatenating I along the side
        MatrixDenseKS<T,NROWS,NCOLS*2> augmentedWI;
        for(int r = 0; r < NROWS; r++){
            for(int c = 0; c < NCOLS*2; c++){
                if(c < NCOLS){
                    augmentedWI.set(r,c, mat[r][c]);
                }else{
                    augmentedWI.set(r,c, ( (c - NCOLS == r) ? T(1.0) : T(0.0) ));
                }
            }
        }
        std::cout << "Augmented matrix: " << augmentedWI << std::endl;

        //Do GJ
        bool notSingular = augmentedWI.gaussElimPartialPiv();

        std::cout << "Slve done " << notSingular << " Augmented matrix: " << augmentedWI << std::endl;


        //Extract
        MatrixDenseKS<T,NROWS,NCOLS> ret;
        for(int r = 0; r < NROWS; r++){
            for(int c = 0; c < NCOLS; c++){
                ret.mat[r][c] = augmentedWI(r,c+NCOLS);
            }
        }

        completedOK = notSingular;
        return ret;
    }

    //TODO: Include general matrix multiplication

    bool operator==(const MatrixDenseKS& rhs)const{
        if(this == &rhs){
            return true;
        }else{
            for(int r = 0; r < NROWS; r++){
                for(int c = 0; c < NCOLS; c++){
                    if(! isClose<T>(mat[r][c], rhs.mat[r][c], DFLT_EPSILON_VAL)){
                        return false;
                    }
                }
            }
            return true;
        }
    }


    bool operator!=(const MatrixDenseKS& rhs)const{
        return !(*this == rhs);
    }

    // \brief Print the matrix
    friend std::ostream& operator<<(std::ostream& os, const MatrixDenseKS<T,NROWS,NCOLS>& m){
        const int PWIDTH = 10;
        os << NROWS << " by " << NCOLS << " dense KS matrix: " << std::endl;
        for(int i = 0; i < NROWS; i++){
            os << '[';
            for(int j = 0; j < NCOLS; j++){
                os << std::fixed << std::setw(PWIDTH) << (m.mat[i][j]) << (j != m.numCols()-1 ? ' ' : ']');
            }
            os << std::endl;
        }
        return os;
    }

private:
    //Private data
    T mat[NROWS][NCOLS];
    //Functions we don't want people calling, but that are automatically defined.

    //Helper functions
    inline void helpCopy(const MatrixDenseKS& other){
        for(int i = 0; i < NROWS; i++){
            for(int j = 0; j < NCOLS; j++){
                mat[i][j] = other.mat[i][j];
            }
        }
    }

    bool helpGaussElimPartialPiv(int indexVec[NROWS], bool reorderOnCompletion){

        //First, we construct an index vector...this allows us to swap rows WITHOUT having to move
        //memory around
        for(int i = 0; i < NROWS; i++){
            indexVec[i] = i;
        }
        //To swap row i and j just do:
        //    int temp = indexVec[i];
        //    indexVec[i] = indexVec[j];
        //    indexVec[j] = temp;

        int c = 0;
        for(int r = 0; r < NROWS-1; r++){
            //Find the BEST row and bring it to the top (using the index vector)
            T maxSoFar   = mat[indexVec[r]][c];
            int idxSoFar = r;
            for(int rr = r+1; rr < NROWS; rr++){
                const T currAbs = absVal<T>(mat[indexVec[rr]][c]);
                if(currAbs > maxSoFar){
                    maxSoFar = currAbs;
                    idxSoFar = rr;
                }
            }
            //Make the row swap
            if(idxSoFar != r){
                const int temp = indexVec[r];
                indexVec[r] = indexVec[idxSoFar];
                indexVec[idxSoFar] = temp;
            }

            //Check for 0 element; we can't continue in this case.  Pivoting avoids this problem.
            const T elem = mat[indexVec[r]][c];
            if(isClose<T>(elem, T(0.0), DFLT_EPSILON_VAL)){ return false; }
            const T invElem  = T(1.0) / elem;
            for(int rr = r + 1; rr < NROWS; rr++){
                //Let b = this elem, we want to make it zero, Let a = the elem in the row above...
                //    b - m * a = 0        m = b/a
                const T m = mat[indexVec[rr]][c] * invElem;
                for(int cc = 0;  cc < NCOLS; cc++){
                    mat[indexVec[rr]][cc] = mat[indexVec[rr]][cc] - (m * mat[indexVec[r]][cc]);
                }
            }
            ++c;
        }

        //We may want to reorder on completion
        //This constitutes APPLYING the index vector, and actually moving things in memory.
        //This gives us an upper triangular matrix.
        if(reorderOnCompletion){

            int indexVecCpy[NROWS];
            for(int i = 0; i < NROWS; i++){
                indexVecCpy[i] = indexVec[i];
            }
            
            for(int i = 0; i < NROWS; i++){
                const int rowToMoveTo = indexVecCpy[i];
                if(rowToMoveTo != i){ //Don't swap if unneeded
                    const bool didSwap = swapRows(rowToMoveTo, i);
                    Assert(didSwap);
                    indexVecCpy[rowToMoveTo] = rowToMoveTo;
                    indexVecCpy[i] = i;
                }
            }
        }
        return true; //Success
    }


};


#undef DFLT_EPSILON_VAL

#endif //MATH_MATRIX_DENSE_KS
