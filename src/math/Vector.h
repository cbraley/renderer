#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <iostream>
#include <cmath>
//--
#include "utils/Assert.h"
#include "utils/Constants.h"

/**
 *  Simple 3D vector class.  Floating point precision.
 *  Components can be manipulated directly via the public member variables
 *  x, y, and z.
 */
class Vector{
public:
    float x; ///< Client has direct access to the x,y, and z components
    float y; ///< Client has direct access to the x,y, and z components
    float z; ///< Client has direct access to the x,y, and z components

    /**
     *  Parameterized constructor for creating arbitrary 3 dimensional vectors.
     *  If no parameters are provided, will take the place of the default
     *  constructor.
     *
     *  Note that the Vector <0,0,0> is degenerate, and will cause assertions to
     *  fail in many other places if operated on.  Before any fucntions are called
     *  on the Vector, at least one component should be non-zero.
     *
     *  @param mX is the x component.  Defaults to 0.0f.
     *  @param mY is the x component.  Defaults to 0.0f.
     *  @param mZ is the x component.  Defaults to 0.0f.
     */
    Vector(float mX = 0.0f, float mY = 0.0f, float mZ = 0.0f);

    /// \brief Copy constructor.
    Vector(const Vector& other);

    /// \brief Get the magnitude of the vector.
    float magnitude()const;

    /**
     *  Normalize the current vector.
     *  @return a reference to the *this vector, this allowing chaining such as:
     *  Vector a,b;
     *  b = a.normalize();
     */
    Vector& normalize();

    /// \brief Get a normalized copy of the vector without modifying the original.
    Vector getNormalized()const;

    /**
     *  Check if a Vector is unit length.  This method allows "fuzzy" checking
     *  for situations in which we want to check if the Vector is very nearly
     *  unit length.
     *
     *  Typically, if we want to normalize
     *  a Vector and are unsure if it is already unit length, it is faster
     *  to just call normalize() again!  However, this method is useful in 
     *  assertions, when we expect a normalized vector and want to verify.
     *
     *  Note that this function allows for some slight deviation from exact
     *  unit length if parameter maxDeviation is non-zero.
     *  @param maxDeviation is the maximum absolute difference from unit length
     *   we allow.  Defaults to 0.00001.
     *  @return true if and only if |1 - magnitude| < maxDeviation.
     */
    bool isNormalized(float maxDeviation = 0.00001f)const;

    /// \brief Simple dot product of two vectors(aka inner product).
    float dot(const Vector& b)const;

    /**
     *  Vector cross product.
     *  @param v2 is the Vector to cross with.
     *  @return (*this) X (v2)
     */
    Vector cross(const Vector& v2)const;

    /// \brief Overloaded unary minus operator is used to flip a vector.
    Vector operator-()const;

    /**
     *  Multiply-and-assign a Vector times a scalar.
     *  @return a reference to *this, with each component
     *  multipled by scale
     */
    Vector& operator*=(float scale);

    /**
     *  Divide-and-assign a Vector by a scalar.
     *  @param scale is a scalar
     *  @return a reference to *this, with each component
     *  divided by scale
     */
    Vector& operator/=(float scale);

    /// \brief Vector add-and-assign.
    Vector& operator+=(const Vector& b);

    /// \brief Vector subtract-and-assign.
    Vector& operator-=(const Vector& b);


    /**
     *  Angle between two vectors using the dot product.
     *  Returns result in radians!
     *  If both vectors are already normalized, this wastes cycles.
     *
     *  Note that using acos( (v1 dot v2)  / (|v1| |v2|) ) is NOT a good approach!  This
     *  is because the range of acos is [-1,1], and due to numerical issues v1 dot v2 will often
     *  be nearly -1 or 1!  This function is implemented in a more numerically stable way.
     *
     *  See here for a numerically well behaved way to implement he angle between two vectors:
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

    /// \brief Return a const ref to the ith component.  i must be in {0,1,2}.
    const float& operator[](int i)const;
    
    /// \brief Return a non-const ref to the ith component.  i must be in {0,1,2}.
    float& operator[](int i);

    //TODO(Colin): Make the == and != functions check for EXACT equality, and write new 
    //named functions for fuzzy testing!  This would make the code a lot more clear when reading,
    //since named functions are self-documenting.

    /// \brief Check two vectors for *fuzzy* equality!  Depends on VECTOR_COMPARE_EPS in
    /// the .cpp file.
    bool operator==(const Vector& b)const;
    /// \brief Check two vectors for *fuzzy* inequality!  Depends on VECTOR_COMPARE_EPS in
    /// the .cpp file.
    bool operator!=(const Vector& b)const;

    /// \brief Make sure the vector has no NaN's("Not-a-Numbers").
    bool hasNoNaNs()const;

    /// \brief Print the vector to a stream.
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec);
    
    /// \brief Read the vector from a stream.  Enter vectors like this:
    /// 1.0 2.1 3.4
    friend std::istream& operator>>(std::istream& is, Vector& vec);
};


//-----------------------------------------------------------------------------
//Binary operators that operate on a vector and a scalar ----------------------
//These must be defined out of the class as non-member(aka "free") functions
//so that we can do expressions like:
//    Vector vec;
//    Vector vec1 = 3.14159f * vec; // (1)
//    Vector vec2 = vec * 3.14159f; // (2)
//If we defined these as member functions, only expression (2) would be valid!
//See:
//    www.keithschwarz.com/cs106l/.../200_Operator_Overloading.pdf
//    http://www.csse.monash.edu.au/~jonmc/CSE2305/Topics/10.19.OpOverload/html/text.html#a_slight_problem_of_ordering
//-----------------------------------------------------------------------------

/// \brief Vector * scalar
inline const Vector operator*(const Vector& v, float scalar) {
    Assert(scalar != 0.0f);
    return Vector(v.x * scalar, v.y * scalar, v.z * scalar);
}
/// \brief scalar * vector
inline const Vector operator*(float scalar, const Vector& v) {
    return v * scalar;
}

/// \brief Vector / scalar
inline const Vector operator/(const Vector& v, float scalar) {
    Assert(scalar != 0.0f);
    const float invScalar = 1.0f / scalar;
    return v * invScalar;
}

//Note: scalar / Vector does not make sense!
//inline const Vector operator/(float scalar, const Vector& v) {
//    return v / scalar;
//}

/// \brief Vector addition.
inline const Vector operator+(const Vector& a, const Vector& b) {
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

/// \brief Vector subtraction.
inline const Vector operator-(const Vector& a, const Vector& b) {
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

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

inline Vector& Vector::operator*=(float scale){
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

inline Vector& Vector::operator/=(float scale){
    Assert(scale != 0.0f);
    float inv = 1.0f / scale;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
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
        return Constants::PI_FLOAT - 2.0f * asin(tempVec.magnitude() * 0.5f);
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

