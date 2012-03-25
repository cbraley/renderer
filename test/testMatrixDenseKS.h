#ifndef TEST_MATRIX_DENSE_KS
#define TEST_MATRIX_DENSE_KS

#include <iostream>
#include <UnitTest++.h>
//--
#include "MatrixDenseKS.h"

/*
SUITE(MatrixDenseKSTests){

    TEST(Construction){
        MatrixDenseKS<float,10,10> I;
        MatrixDenseKS<float,10,10> Icopy(I);
        MatrixDenseKS<float,10,10> a = Icopy;
        CHECK(I == Icopy);
    }

    TEST(MatrixScalarOps){
        MatrixDenseKS<double,4,4> I;

        //Scalar multiplication
        MatrixDenseKS<double,4,4> I4 = I * 4.0;
        CHECK_EQUAL(4.0, I4(0,0));
        CHECK_EQUAL(4.0, I4(1,1));
        CHECK_EQUAL(4.0, I4(1,1));
        CHECK_EQUAL(4.0, I4(1,1));
        CHECK_EQUAL(0.0, I4(0,1));
    }

    TEST(MatrixMatrixOps){
        MatrixDenseKS<float,2,2> m;
        MatrixDenseKS<float,2,2> m2 = m + m - m;
        std::cout << m2 << std::endl;
    }

    TEST(LinearSolve){
        std::cout << "TEST LINEAR SOLVE" << std::endl;
        const float EPSILON_VAL = 0.0001f;
        const float inA[5][5] = {
            {1, 2,  3, 4, 5 },
            {0, 1, -1, 4, 5 },
            {1, 1,  2, 3, -1},
            {5, 0,  2, 4, 7 },
            {4, 5,  6, 7, 0 }
        };
        const float inRHS[5] = {1, 2, 3, 4, 7};
        const float inXex[5] = {40.0f/33.0f, -43.0f/231.0f, -39.0f/77.0f, -202.0f/231.0f, -4.0f/11.0f };
        VectorDenseKS<float,4> xEx(inXex);

        MatrixDenseKS<float,5,5> A(inA);
        VectorDenseKS<float,5>   b(inRHS);
        bool nonSingular;
        VectorDenseKS<float,5>   xSolve = 
            A.solveLinearSystem(b, nonSingular);
        std::cout << "Non-Singular = " << nonSingular << std::endl;
        std::cout << "xSolve = "   << xSolve << std::endl;       
        std::cout << "xExpected = " << xEx   << std::endl;       
 

        CHECK(nonSingular);
        CHECK_CLOSE(xSolve(0), inXex[0], EPSILON_VAL);
        CHECK_CLOSE(xSolve(1), inXex[1], EPSILON_VAL);
        CHECK_CLOSE(xSolve(2), inXex[2], EPSILON_VAL);
        CHECK_CLOSE(xSolve(3), inXex[3], EPSILON_VAL);
        CHECK_CLOSE(xSolve(4), inXex[4], EPSILON_VAL);
        

        std::cout << "END TEST LINEAR SOLVE" << std::endl;
    }
}
*/

#endif //TEST_MATRIX_DENSE_KS
