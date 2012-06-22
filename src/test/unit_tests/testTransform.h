#ifndef TEST_TRANSFORM_H
#define TEST_TRANSFORM_H

#include <UnitTest++.h>
//--
#include "math/Transform.h"
#include "math/Vector.h"
#include "math/Point.h"
#include "math/Normal.h"

SUITE(TransformTests){

    #define EPSILON 0.0001f
    #define CHECK_CLOSE_GEOM(a,b,eps) CHECK_CLOSE(a.x,b.x,eps);CHECK_CLOSE(a.y,b.y,eps);CHECK_CLOSE(a.z,b.z,eps);

    /*
    bool close(const Vector& v1, const Vector& v2, float EPSILON = 0.0001f){
        return fabsf(v1.x - v2.x) < EPSILON &&
            fabsf(v1.y - v2.y) < EPSILON &&
            fabsf(v1.z - v2.z) < EPSILON;
    }

    bool close(const Point& p1, const Point& p2, float EPSILON = 0.0001f){
        return fabsf(p1.x - p2.x) < EPSILON &&
            fabsf(p1.y - p2.y) < EPSILON &&
            fabsf(p1.z - p2.z) < EPSILON;
    }
    */

    /*

    TEST(TransformConstructors){
        Transform I1;
        float data[4][4] = { 
                {1.0f , 0.0f ,0.0, 0.0f},
                {0.0f , 1.0f ,0.0, 0.0f},
                {0.0f , 0.0f ,1.0, 0.0f},
                {0.0f , 0.0f ,0.0, 1.0f}
            };
        Transform I2(data, data);

        CHECK(I1 == I2);
        CHECK(I1.inverse() == I1);
        CHECK(I2.inverse() == I2);
        CHECK(I2.inverse() == I1);
        CHECK(I1.inverse() == I2);
    }


    TEST(TransformVectors){
        Vector v(1.0f, 0.0f, 0.0f);
        Transform moveUp = Transform::translate(
            Vector(2.0f, 0.0f, 0.0f));
        Vector v2 = moveUp(v);
        CHECK(v == v2);

        Transform moveAway = Transform::translate(
            Vector(.9f,-1.2f, 2.1f));
        Vector v3 = moveAway(v);
        CHECK(v == v3);
    }
    */

    /*
    TEST(RotationTransforms){
        Transform spinAroundZ = Transform::rotateZ(180.0f);
        Point p1(0.0f, 10.0f, -55.0f);
        Point p2 = spinAroundZ(p1);
        CHECK_CLOSE_GEOM(Point(0.0f, -10.0f, -55.0f), p2, EPSILON);

    }
    */

/*
    TEST(TranformationComposition){

    }


    TEST(TransformNormals){
    }

    TEST(TransformBBoxs){
    }
    */
}

#endif //TEST_TRANSFORM_H
