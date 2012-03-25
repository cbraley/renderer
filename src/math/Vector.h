#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <iostream>
#include <cmath>
//--
#include "Assert.h"
#include "Constants.h"


/**
 *  Simple 3D vector class.  Floating point precision.
 *  Components can be manipulated directly via the public member variables
 *  x, y, and z.
 *  Cross product assumes a left handed coordinate system.
 */
class Vector{
public:
    //Public access
    float x, y, z;

    /**
     *  Parameterized/default c-tor.
     *  @param mX is the x component.  Defaults to 0.0f.
     *  @param mY is the x component.  Defaults to 0.0f.
     *  @param mZ is the x component.  Defaults to 0.0f.
     */
    Vector(float mX = 0.0f, float mY = 0.0f, float mZ = 0.0f);

    /**
     *  Copy constructor.
     *  @param other is the vec to copy.
     */
    Vector(const Vector& other);


    /**
     *  Get the magnitude of a given vector.
     *  @return magnitude
     */
    float magnitude()const;

    /**
     *  Normalize the current vector.
     *  @return a reference to the *this vector, this allowing chaining such as:
     *  Vector a,b;
     *  b = a.normalize();
     */
    Vector& normalize();

    /**
     *  Get a normalized copy of the vector without modifying the original vec.
     *  @return a new normalized vector.
     */
    Vector getNormalized()const;

    /**
     *  Simple dot (inner) product.
     *  @param b is the second vector in the dot product.
     *  @return the dot product of two vectors
     */
    float dot(const Vector& b)const;

    /**
     *  Vector cross product.
     *  Remember we use a left handed coordinate system.
     *  @param v2 is the Vector to cross with.
     *  @return (*this) X (v2)
     */
    Vector cross(const Vector& v2)const;

    /**
     *  Multiplty a vector by a scalar and produce a new vector.
     *  @param scale is the multiplier
     *  @return a new vector
     */
    Vector operator*(float scale)const;
    friend Vector operator*(float scale, const Vector& v);

    Vector& operator*=(float scale);

    Vector operator/(float scale)const;


    Vector& operator/=(float scale);


    /**
     *  Vector addition.
     */
    Vector operator+(const Vector& b);

    Vector& operator+=(const Vector& b);

    /**
     *  Overloaded unary minus operator is used to flip a vector.
     */
    Vector operator-()const;

    /**
     *  Vector subtraction.
     */
    Vector operator-(const Vector& b);

    Vector& operator-=(const Vector& b);

    /**
     *  Angle between two vectors
     *  by using the dot product.  Returns result in radians!
     *
     *  If both vectors are already normalized, this wastes cycles.
     *
     *  Note that using acos( (v1 dot v2)  / (|v1| |v2|) ) is NOT a good approach!  This
     *  is because the range of acos is [-1,1], and due to numerical issues v1 dot v2 will often
     *  be nearly -1 or 1!
     *
     *  See here for a numerically well behaved way to implementt he angle between two vectors:
     *  http://www.plunk.org/~hatch/rightway.php
     *  Credit goes to the authors of the "Mitsuba" renderer whose source code pointed me to this resource.
     */
    float angle(const Vector& b)const;

    /**
     *  Cmpute the angle between two pre-normalized vectors.  
     *  Not computed in the naive way, see the comments on the "angle" method.
     *
     *  Both *this and b MUST be normalized prior to calling this function.
     */
    float angleNormalized(const Vector& b)const;

    const float& operator[](int i)const;

    float& operator[](int i);

    bool operator==(const Vector& b)const;

    bool operator!=(const Vector& b)const;

    bool hasNoNaNs()const;

    friend std::ostream& operator<<(std::ostream& os, const Vector& vec);
    //Vectors are entered like:
    // 1 2 3
    //to get the vector <1,2,3>
    friend std::istream& operator>>(std::istream& is, Vector& vec);

    bool isNormalized()const;

};

//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------

inline Vector::Vector(float mX, float mY, float mZ) :
    x(mX), y(mY), z(mZ)
{
    Assert(hasNoNaNs());
}

inline Vector::Vector(const Vector& other) :
    x(other.x), y(other.y), z(other.z)
{
    Assert(hasNoNaNs());
}

inline float Vector::magnitude()const{
    return sqrtf(x*x + y*y + z*z);
}

inline Vector& Vector::normalize(){
    const float invMag = 1.0f / magnitude();
    x *= invMag;
    y *= invMag;
    z *= invMag;
    return *this;
}

inline Vector Vector::getNormalized()const{
    const float invMag = 1.0f / magnitude();
    return Vector(x * invMag, y * invMag, z * invMag);
}

inline float Vector::dot(const Vector& b)const{
    return x * b.x + y * b.y + z * b.z;
}

inline Vector Vector::cross(const Vector& v2)const{
    //The above is LEFT HANDED
    //(OpenGL is right handed)
    //(Renderman is left handed)
    return Vector(
        (y * v2.z) - (z * v2.y),
        (z * v2.x) - (x * v2.z),
        (x * v2.y) - (y * v2.x)
        );
}

inline Vector Vector::operator*(float scale)const{
    return Vector(x * scale, y * scale, z * scale);
}
inline Vector operator*(float scale, const Vector& v){
    return Vector(v.x * scale, v.y * scale, v.z * scale);
}

inline Vector& Vector::operator*=(float scale){
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

inline Vector Vector::operator/(float scale)const{
    Assert(scale != 0.0f);
    float inv = 1.0f / scale;
    return Vector(x * inv, y * inv, z * inv);
}

inline Vector& Vector::operator/=(float scale){
    Assert(scale != 0.0f);
    float inv = 1.0f / scale;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

inline Vector Vector::operator+(const Vector& b){
    return Vector(x+b.x, y+b.y, z+b.z);
}

inline Vector& Vector::operator+=(const Vector& b){
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

inline Vector Vector::operator-()const{
    return Vector(-x,-y,-z);
}

inline Vector Vector::operator-(const Vector& b){
    return Vector(x-b.x, y-b.y, z-b.z);
}

inline Vector& Vector::operator-=(const Vector& b){
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return *this;
}

inline float Vector::angle(const Vector& b)const{

    //My original implmentation used clamping here...however,
    //clamping is not a good apprach!
    //http://www.plunk.org/~hatch/rightway.php
    /*
    const float den = magnitude() * b.magnitude();
    const float dp = dot(b);
    float x = dp/den;
    x = x >= 1.0f ? 1.0f : (x <= -1.0f ? -1.0f : x); //Clamp x to [-1,1]
    const float ret = acos(x);
    Assert(ret == ret);
    return ret;
    */

    const Vector temp1 =   getNormalized();
    const Vector temp2 = b.getNormalized();
    return temp1.angleNormalized(temp2);
}


inline float Vector::angleNormalized(const Vector& b)const{
    const float dp = dot(b);
    if(dp < 0.0f){
        const Vector tempVec(-x - b.x, -y - b.y, -z - b.z);
        return PI_FLOAT - 2.0f * asin(tempVec.magnitude() * 0.5f);
    }else{
        const Vector tempVec(x - b.x, y - b.y, z - b.z);
        return 2.0f * asin(tempVec.magnitude() * 0.5f);
    }
}

inline const float& Vector::operator[](int i)const{
    Assert(i >= 0 && i < 3);
    if(i == 0){
        return x;
    }else if(i == 1){
        return y;
    }else{
        return z;
    }
}

inline float& Vector::operator[](int i){
    Assert(i >= 0 && i < 3);
    if(i == 0){
        return x;
    }else if(i == 1){
        return y;
    }else{
        return z;
    }
}

inline bool Vector::hasNoNaNs()const{
    return !(isnan(x) || isnan(y) || isnan(z));
}

#endif //VECTOR_H

