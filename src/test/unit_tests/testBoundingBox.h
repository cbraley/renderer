#ifndef TEST_BB_H
#define TEST_BB_H

#include <UnitTest++.h>
//--
#include <iostream>
//--
#include "geom/BoundingBox.h"
#include "math/Point.h"

SUITE(BoundingBox){

    TEST(BoundingBoxConstruction){
        BoundingBox bb;
        bb.Union( Point(10,10,10) );
        bb.Union( Point(-10,-10,-10) );
    }

    TEST(BoundingBoxCorners){
        BoundingBox aabb(Point(10,10,10), Point(-10,-10,-10));
        Point corners[8];
        aabb.getCorners(corners);
        for(int i = 0; i < 8; i++){
            //std::cout << corners[i] << std::endl;
        }

    }

}

#endif //TEST_BB_H
