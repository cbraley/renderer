#ifndef TEST_SPHERE_H
#define TEST_SPHERE_H

#include <UnitTest++.h>
//--
#include "primitives/Sphere.h"
#include "primitives/Shape.h"
#include "math/Transform.h"
#include "math/Vector.h"
#include "math/Point.h"
#include "geom/Ray.h"

SUITE(SphereTests){
    TEST(RaySphereIntersectSimple){
        //Sphere @ origin with radius 3
        Shape* s = new Sphere(Transform(), 1.5f);
        Ray r(Point(0.0f, 0.0f, -10.0f), Vector(0.0f, 0.0f, 1.0f));
        RaySurfIntersection hitLoc;
        bool hit = s->intersect(r, hitLoc);

        //Make sure we hit
        CHECK(hit);

        //Make sure the hit is correct
        const float EPS = 0.00001f;
        CHECK_CLOSE(10.0f - 1.5f, hitLoc.tHit, EPS);

        delete s;
    }
}

#endif //TEST_SPHERE_H
