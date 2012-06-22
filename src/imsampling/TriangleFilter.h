#ifndef TRIANGLE_FILTER_H
#define TRIANGLE_FILTER_H

#include <cmath>
//--
#include "imsampling/ReconstructionFilter.h"
#include "utils/Assert.h"

class TriangleFilter : public ReconstructionFilter{
private:
    float xw, yw;
    float invXw, invYw;

    inline float evalTriangle(float v, float w, float invW)const{
        return (w - fabs(v)) * invW;
    }

public:

    TriangleFilter(float xWidth = 1.0f, float yWidth = 1.0f) :
        xw(xWidth), yw(yWidth),
        invXw(1.0f/xWidth), invYw(1.0f/yWidth)
    {
        Assert(xw > 0.0f && yw > 0.0f);
    }

    virtual float evalFilter(float xLoc, float yLoc)const{
        return evalTriangle(xLoc, xw, invXw) *
            evalTriangle(yLoc, yw, invYw);
    }

    virtual float extentX()const{return xw;}
    virtual float extentY()const{return yw;}

};

#endif //TRIANGLE_FILTER_H
