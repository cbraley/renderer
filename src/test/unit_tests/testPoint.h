#ifndef TEST_POINT_H
#define TEST_POINT_H

#include <UnitTest++.h>
//--
#include "math/Point.h"

SUITE(PointTests){

    TEST(Point){
        CHECK(Point(1.0f,0.0f,0.0f) == Point(1.0f,0.0f,0.0f));
    }

}

#endif //TEST_POINT_H
