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
        CHECK(tri1 != NULL);
        CHECK(tri2 != NULL);
        CHECK(tri3 != NULL);

        try{
            std::cout << "A" << std::endl;
            BVH bvh(BVH::SPLIT_CENTER, 1);
            CHECK(bvh.getPartitionStrategy() == BVH::SPLIT_CENTER);

            std::vector<Shape*> shapes;
            shapes.push_back(tri1);
            shapes.push_back(tri2);
            shapes.push_back(tri3);
            std::cout << "B" << std::endl;

            bvh.buildAccelStructure(shapes);
            std::cout << "C" << std::endl;

            BVH::PrintVisitor* v = new BVH::PrintVisitor(std::cout);
            bvh.traverse(v);
            delete v;
        }catch(std::exception ex){
            std::cerr << "Exception - : " << ex.what() << std::endl;
            CHECK(false);
        }
    }

}

#endif //TEST_BVH_H
