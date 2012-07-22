#ifndef TEST_BVH_H
#define TEST_BVH_H

#include <UnitTest++.h>
//--
#include "accel/BVH.h"
#include "primitives/Triangle.h"

SUITE(BVHTests){

    TEST(TestSimpleBVH){
        Shape* tri1 = new Triangle(
            Point(0,1,0),
            Point(0,1,2),
            Point(1,2,3)
            );
        Shape* tri2 = new Triangle(
            Point(4,1,0),
            Point(5,1,2),
            Point(1,6,3)
            );
        Shape* tri3 = new Triangle(
            Point(0,-9,0),
            Point(0,-8,2),
            Point(1,-3,3)
            );
        BVH bvh(BVH::SPLIT_CENTER, 1);

        std::vector<Shape*> shapes;
        shapes.push_back(tri1);
        shapes.push_back(tri2);
        shapes.push_back(tri3);

        bvh.buildAccelStructure(shapes);
    }

}

#endif //TEST_BVH_H
