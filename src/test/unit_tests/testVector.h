#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include <UnitTest++.h>
//--
#include "math/Vector.h"
#include "math/MathUtils.h"

SUITE(VectorTests){

    TEST(VectorConstructorsAndSimpleOps){
        for(int i = 0; i < 3; i++){
            Vector v;
            v[i] = 1.0f;
            CHECK(v.magnitude() == 1.0f);
            CHECK(v.getNormalized() == v);
        }

        //Simple parameterized c-tor
        Vector v(-2.0f, 2.0f, 3.0f);
        CHECK_EQUAL(sqrtf(4.0f + 4.0f + 9.0f), v.magnitude());
    }

    TEST(VectorAngle){
        const float DEGREE_EPS = 0.001f;
        Vector a,b;

        //Two vectors same direction different lengths
        a = Vector(100.0f, 22.0f, -66.0f);
        a.normalize();
        b = a * 5.0f;
        CHECK_CLOSE( MathUtils::radsToDegrees(a.angle(b)), 0.0f, DEGREE_EPS);

        b = -a;
        CHECK_CLOSE( MathUtils::radsToDegrees(a.angle(b)), 180.0f, DEGREE_EPS);

    }

}

#endif //TEST_VECTOR_H
