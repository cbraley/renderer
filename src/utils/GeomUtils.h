#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

#include "Point.h"
#include "Vector.h"
#include "RNGs.h"
//--
#include <cmath>

/**
 *  Miscellaneous short helper functions for operations on geometric entities
 *  such as points, vectors, rays, and planes.
 *
 *  @date Jan 9 2012
 */
namespace GeomUtils{


    /**
     *  Return true if a set of 3 points is colinear; false otherwise.
     *  Optional parameter for controlling precision.
     */
    inline bool arePointsColinear(const Point& p1, const Point& p2, const Point& p3, float epsTol = 0.00001f){

        //Compute direction vector from p1 to p2, and vector from p1 to p3
        Vector dir1 = (p2 - p1).getNormalized();
        Vector dir2 = (p3 - p1).getNormalized();

        //If these vectors point in the same direction, or are negations of each other
        //then the points are colinear
        //
        //Compute angle between the two vectors(radians)
        const float theta = acos( dir1.dot(dir2) );

        //If theta is near 0.0f, or near M_PI, then the three points are colinear
        return fabs(theta - 0.0f) <= epsTol || fabs(theta - M_PI) <= epsTol;
    }


    /**
     *  Compute the "scalar triple product" of three 3D vectors.  The scalar triple product
     *  gives the volume of the parallelepiped defined by the 3 vectors.  If this volume
     *  is equal to 0(while accounting for some numerical imprecision, so say < EPSILON),
     *  then the three Vectors are coplanar.  For more info, see:
     *  http://en.wikipedia.org/wiki/Triple_product
     *
     *  Warning - This value is signed!
     *
     *  @param v1 first vector
     *  @param v2 second vector
     *  @param v3 third vector
     *  @return scalar triple product dot(v1, cross(v2,v3) )
     */
    inline float scalarTripleProduct(const Vector& v1, const Vector& v2, const Vector& v3){
        return v1.dot( v2.cross(v3) );
    }



    /**
     *  Absolute value of dot product.
     *  @param a is vector 1
     *  @param b is vector 2
     *  @return |a . b|
     */
    inline float absDotProd(const Vector& a, const Vector& b){
        return std::abs<float>(a.dot(b));
    }


    /**
     *  Create a Vector from a normal.
     *  @param n is the normal.
     */
    inline Vector vectorFromNormal(const Normal& n){ return Vector(n.x, n.y, n.z); }

    inline Vector randomVecOnSphere(){
        const float phi   = RNG::randomFloat(0.0f, TWO_PI_FLOAT);
        const float theta = RNG::randomFloat(0.0f, TWO_PI_FLOAT);
        return Vector(
            sinf(theta) * cosf(phi),
            sinf(theta) * sinf(phi),
            cosf(theta)
        );
    }



    /**
     *  Compute the "ideal specular reflection" direction, aka the direction
     *  of a mirror reflection.
     *
     *  @param in MUST BE NORMALIZED when calling this function.
     */
    inline Vector mirrorRefl(const Vector& in, const Normal& n){
        Assert( fabs(in.magnitude() - 1.0f) < 0.0001f );
        const Vector nTemp = GeomUtils::vectorFromNormal(n);
        const Vector vec = 2.0f * (nTemp.dot(in)) * nTemp;
        const Vector reflDir( vec.x - in.x, vec.y - in.y, vec.z - in.z);
        return reflDir.getNormalized();
    }



}

#endif //GEOM_UTILS_H
