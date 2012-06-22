#ifndef BOX_FILTER_H
#define BOX_FILTER_H

#include "imsampling/ReconstructionFilter.h"

/**
 *  Simplest possible reconstruction filter.
 *  All samples are weighted equally; regardless of their position.
 */
class BoxFilter : public ReconstructionFilter{
private:
    float w,h;
public:
    
    BoxFilter(float width = 1.0f, float height = 1.0f) : 
        w(width), h(height)
    {}

    virtual float evalFilter(float xLoc, float yLoc)const{
        const float distX = fabsf(xLoc);
        const float distY = fabsf(yLoc);
        return (distX <= w && distY <= h) ? 
            1.0f : 0.0f;
    }

    virtual float extentX()const{return w;}
    virtual float extentY()const{return h;}

};


#endif //BOX_FILTER_H
