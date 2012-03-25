#ifndef JITTERED_SAMPLER_H
#define JITTERED_SAMPLER_H

#include "ImageSampler.h"

class JitteredSampler : public ImageSampler{
protected:
    float jitX, jitY; //How much to jitter in each direction
    
public:
   
    /**
     *  Construct a JitteredSampler with a specified width, height, 
     *  number of pixel samples, and jitter amount.  If used with 
     *  default jitter parameters of -1.0f, then the jitter amount will be 
     *  computed automatically so that each sample is jittered to lie within
     *  the bounds of its pixel.  Otherwise, jitter amounts must be >= 0.
     */
    JitteredSampler(int imageWidth, int imageHeight, int sampsPerPixel = 1,
        float jitterX = -1.0f, float jitterY = -1.0f);

    virtual void getSamplesForSubwindow(int winLeft, int winRight,
        int winTop, int winBottom, ImageSampler::Sample*& sampsGoHere)const;

};


#endif //JITTERED_SAMPLER_H
