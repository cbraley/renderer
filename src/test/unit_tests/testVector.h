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

    TEST(VectorScalarOps){
        
        Vector v(1.0f, 2.0f, 3.0f);
        Vector vOrig(1.0f, 2.0f, 3.0f);
        v *= 1.0f;
        CHECK_EQUAL(1.0f, v.x);
        CHECK_EQUAL(2.0f, v.y);
        CHECK_EQUAL(3.0f, v.z);
        v /= 2.0f;
        CHECK_EQUAL(0.5f, v.x);
        CHECK_EQUAL(1.0f, v.y);
        CHECK_EQUAL(1.5f, v.z);

        // The below binary operators wouldn't work if they were written
        // as member functions
        Vector b = 2.0f * v;
        v = vOrig;
        CHECK_EQUAL(v, b);

        Vector c = 3.0f * v;
        CHECK_EQUAL(3.0f, c.x);
        CHECK_EQUAL(6.0f, c.y);
        CHECK_EQUAL(9.0f, c.z);

        // Vector / scalar is more often written as Vector * (1.0/sclar)
        // but we allow the division operator...
        Vector d = c / 3.0f;
        CHECK_EQUAL(1.0f, d.x);
        CHECK_EQUAL(2.0f, d.y);
        CHECK_EQUAL(3.0f, d.z);
    }

}

#endif //TEST_VECTOR_H
