#ifndef GAUSSIAN_FILTER_H
#define GAUSSIAN_FILTER_H

#include <cmath>
//--
#include "ReconstructionFilter.h"
#include "Assert.h"

class GaussianFilter : public ReconstructionFilter{
private:
    float alpha, xw, yw;
    float xExp, yExp; //Precomputed x and y exponents

    inline float gauss(float v, float expV)const{
        return std::max<float>(0.0f, exp(-alpha * v * v) - expV);
    }
public:

    GaussianFilter(float alphaVal, float xWidth = 1.0f, float yWidth = 1.0f) : 
        alpha(alphaVal), xw(xWidth), yw(yWidth)
    {
        Assert(alpha > 0.0f && xw > 0.0f && yw > 0.0f);
        xExp = exp(-alpha * xw * xw);
        yExp = exp(-alpha * yw * yw);
    }

    virtual float evalFilter(float xLoc, float yLoc)const{
        //Works because 2D gaussian is seperable
        return gauss(xLoc, xExp) * gauss(yLoc, yExp);
    }


    virtual float extentX()const{return xw;}
    virtual float extentY()const{return yw;}

};

#endif //GAUSSIAN_FILTER_H
