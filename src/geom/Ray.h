#ifndef RAY_H
#define RAY_H

#include <string>
#include <iostream>
//--
#include "math/Vector.h"
#include "math/Point.h"
#include "utils/Assert.h"
#include "utils/Constants.h"

//static const float DFLT_RAY_MOVE_EPSILON = MIN_POS_FLOAT * 100.0f;
//static const float DFLT_RAY_MOVE_EPSILON = 0.00022f;
static const float DFLT_RAY_MOVE_EPSILON = 0.01000f;

class Ray{
public:

    //Default c-tor, makes the ray (0,0,0) + <0,0,1>
    Ray();
    Ray(const Point& origin, const Vector& dirVec, bool dirIsNormalized = false);
    Ray(const Ray& other);

    Point getOrigin()const;
    void setOrigin(const Point& origin);
    Vector getDir()const;
    void setDir(const Vector& direction, bool dirIsNormalized = false);

    //Unary minus operator for flipping a Ray
    Ray operator-()const;


    /**
     *  Evaluate the ray at a certain distance from it's origin.
     */
    Point operator()(const float t)const;

    /**
     *  To avoid the classic self-intersection problem caused by 
     *  computer arithmetic this function allows you to move a
     *  Ray's origin in the direction of the ray itself by a very slight 
     *  amount.
     */
    void moveOriginSlight(float moveEps = DFLT_RAY_MOVE_EPSILON);

    bool operator==(const Ray& other)const;
    bool operator!=(const Ray& other)const;

    //IO
    friend std::ostream& operator<<(std::ostream& os, const Ray& r);

private:
    Vector dir;
    Point o;
};

//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------
//Default c-tor, makes the ray (0,0,0) + <0,0,1>
inline Ray::Ray() :
    dir(Vector(0.0f,0.0f,1.0f)), o(0.0f,0.0f,0.0f){}

inline Ray::Ray(const Point& origin, const Vector& dirVec, bool dirIsNormalized):
    dir(dirVec), o(origin){
    if(!dirIsNormalized){
        dir.normalize();
    }
    Assert(dir.hasNoNaNs());
}

inline Ray::Ray(const Ray& other) : dir(other.dir), o(other.o){
    Assert(dir.hasNoNaNs());
}

inline Point Ray::getOrigin()const{ return o; }
inline void Ray::setOrigin(const Point& origin){ o = origin; }
inline Vector Ray::getDir()const{ return dir; }

inline void Ray::setDir(const Vector& direction, bool dirIsNormalized){
    if(dirIsNormalized){
        dir = direction;
    }else{
        dir = direction.getNormalized();
    }
}

inline Point Ray::operator()(const float t)const{ return o + (dir * t); }

inline void Ray::moveOriginSlight(float moveEps){
    o = (*this)(moveEps);
}


inline Ray Ray::operator-()const{
    return Ray(o, -dir, true);
}

#endif //RAY_H
