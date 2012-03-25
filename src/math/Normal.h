#ifndef NORMAL_H
#define NORMAL_H

#include <string>
#include <iostream>
#include <cmath>
#include <cassert>
//--
#include "Config.h"
#include "Vector.h"
#include "Assert.h"


//TODO: Make == and != and = for all these basic classes

//Note:  Normals don't transform the same way vectors do!
class Normal{
public:
    float x, y, z;

    //Constructors
    Normal(float mX = 0.0f, float mY = 0.0f, float mZ = 0.0f,
        bool normalized = false);


    Normal(const Normal& other);

    explicit Normal(const Vector& vec, bool normalized = false);

    Normal operator-()const;

    bool operator==(const Normal& other)const;

    bool operator!=(const Normal& other)const;
    
    //IO
    friend std::ostream& operator<<(std::ostream& os, const Normal& vec);


    //Normals are entered like:
    // 1 2 3
    //to get the normal <1,2,3>
    friend std::istream& operator>>(std::istream& is, Normal& vec);


private: //Helper functions

    //Make sure no NaN values crept in
    inline bool hasNoNaNs()const{
        return !(isnan(x) || isnan(y) || isnan(z));
    }

    //User should never call this, Normals are always normalized
    //from the perspective of the client code
    inline float magnitude()const{
        return sqrtf(x*x + y*y + z*z);
    }

    //User should never call this, Normals are always normalized
    //from the perspective of the client code
    inline Normal& normalize(){
        const float mag = magnitude();
        if(mag != 0.0f){
            const float invMag = 1.0f / mag;
            x *= invMag;
            y *= invMag;
            z *= invMag;
        }
        return *this;
    }
};

//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------


//Constructors
inline Normal::Normal(float mX, float mY, float mZ,
    bool normalized) :
    x(mX), y(mY), z(mZ)
{
    if(!normalized){
        normalize();
    }
    Assert(hasNoNaNs());
}

inline Normal::Normal(const Normal& other) :
    x(other.x), y(other.y), z(other.z)
{
    Assert(hasNoNaNs());
}

inline Normal::Normal(const Vector& vec, bool normalized) :
    x(vec.x), y(vec.y), z(vec.z)
{
    if(!normalized){
        normalize();
    }
}


inline Normal Normal::operator-()const{
    return Normal(-x, -y, -z);
}


#endif //NORMAL_H

