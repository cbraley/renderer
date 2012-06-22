#ifndef TEST_MATRIX_UTILS_H
#define TEST_MATRIX_UTILS_H

#include <UnitTest++.h>
#include <iostream>
//--
#include "math/MatrixUtils.h"

using namespace MatrixUtils;

SUITE(TestMatrixUtils){
    
    /*
    TEST(GaussJordanInvert){

        float EPSILON_V = 0.01f;

        float inputData[5][5] = {
            {1.000f,  2.000f,  3.000f,  4.000f,  5.000f,},
            {0.000f,  1.000f,  3.000f,  5.000f,  6.000f,},
            {0.000f,  0.000f,  0.000f,  1.000f,  2.000f,},
            {5.000f,  6.000f,  7.000f,  8.000f,  0.000f,},
            {1.000f,  0.000f,  1.000f, -1.000f,  0.000f,}
        };

        float expectedOutputData[5][5] = {
            {-0.536f, -0.107f,  1.661f,  0.196f,  0.554f,},
            {1.250f, -0.750f, -0.875f, -0.125f, -0.625f,},
            {-0.036f,  0.393f, -1.089f, -0.054f,  0.304f,},
            {-0.571f,  0.286f,  0.571f,  0.143f, -0.143f,},
            {0.286f, -0.143f,  0.214f, -0.071f,  0.071f,}
        };

        float** A          = allocMatrix<5,5,float>();
        float** AinvActual = allocMatrix<5,5,float>();

        setMatrixData<5,5,float>(A,inputData);
        setMatrixData<5,5,float>(AinvActual,expectedOutputData);

        //Calculate the inverse and compare
        float** Ainv = gaussJordanInvert<5,float>(A);
        bool cmp = equalityWithinEpsilon<5,5,float>(Ainv, AinvActual, EPSILON_V);
        CHECK(cmp );
   
        if(!cmp){
            std::cout << "Ainv Result:" << std::endl;
            printMat<5,5,float>(std::cout, Ainv);
            std::cout << "Ainv Expected:" << std::endl;
            printMat<5,5,float>(std::cout, AinvActual);
        }

        freeMatrix<5,5,float>(A);
        freeMatrix<5,5,float>(Ainv);
        freeMatrix<5,5,float>(AinvActual);
    }

    TEST(GaussJordanElim){ //Example from page 84 of Strang's linear algebra
        const double MATRIX_EPSILON = 0.01;
        double A[3][6] = {
            {2,-1,0,1,0,0 },
            {-1,2,-1,0,1,0},
            {0,-1,2,0,0,1 }
        };
        double B[3][6] = {
            {1,0,0, .75, .5, .25 },
            {0,1,0, .5, 1, .5 },
            {0,0,1, .25, .5, .75 },
        };
        double** mat = allocMatrix<3,6,double>();
        double** expected = allocMatrix<3,6,double>();
        CHECK(mat != NULL);
        fillMatrix<3,6,double>(mat,0.0);
        setMatrixData<3,6,double>(mat, A);
        setMatrixData<3,6,double>(expected, B);
        gaussJordanElim<3,6,double>(mat);

        bool ok = equalityWithinEpsilon<3,6,double>(mat, expected, MATRIX_EPSILON);
        CHECK(ok);       
        freeMatrix<3,6,double>(mat);
        freeMatrix<3,6,double>(expected);
    }
    */
}

#endif //TEST_MATRIX_UTILS_H
