#ifndef POINT_H
#define POINT_H

#include "Vector.h"
#include "Assert.h"

/**
 *  Simple 3D point class.
 *  @author Colin Braley
 */


class Point{
public:
    float x,y,z;

    Point(float mX = 0.0f, float mY = 0.0f, float mZ = 0.0f);

    Point(const Point& other);

    explicit Point(const Vector& vec);

    //Operations
    Point operator+(const Vector& b)const;

    Point& operator+=(const Vector& b);

    Vector operator-(const Point& b)const;

    Point& operator-=(const Vector& v);

    bool hasNoNaNs()const;

    float distance(const Point& b)const;


    bool operator==(const Point& other)const;
    bool operator!=(const Point& other)const;

    //IO
    friend std::ostream& operator<<(std::ostream& os, const Point& p){
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }

    //Points are entered like:
    // 1 2 3
    //to get the point (1,2,3)
    friend std::istream& operator>>(std::istream& is, Point& p){
        return is >> p.x >> p.y >> p.z;
    }
};

//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------

inline Point::Point(float mX, float mY, float mZ) :
    x(mX), y(mY), z(mZ)
{
    Assert(hasNoNaNs());
}

inline Point::Point(const Point& other) :
    x(other.x), y(other.y), z(other.z)
{
    Assert(hasNoNaNs());
}

inline Point::Point(const Vector& vec) :
    x(vec.x), y(vec.y), z(vec.z)
{
    Assert(hasNoNaNs());
}

inline Point Point::operator+(const Vector& b)const{
    return Point(x+b.x, y+b.y, z+b.z);
}

inline Point& Point::operator+=(const Vector& b){
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

inline Vector Point::operator-(const Point& b)const{
    return Vector(x-b.x, y-b.y, z-b.z);
}

inline Point& Point::operator-=(const Vector& v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

inline bool Point::hasNoNaNs()const{
    return !(isnan(x) || isnan(y) || isnan(z));
}

inline float Point::distance(const Point& b)const{
    return (*this - b).magnitude();
}

#endif //POINT_H

