#ifndef TEST_JITTERED_SAMPLER_H
#define TEST_JITTERED_SAMPLER_H

#include <UnitTest++.h>
//--
#include "imsampling/JitteredSampler.h"
#include "imsampling/ImageSampler.h"

SUITE(JitteredSamplerTests){

    TEST(Basics){
        const int NUM_TRIALS = 32;

        int w = 101;
        int h = 102;
        int ssp = 10;
        float jitX = 2.0f;
        float jitY = 5.0f;
        ImageSampler* js = new JitteredSampler(w,h,ssp, jitX, jitY);

        //Check the basics
        CHECK_EQUAL(w, js->getImageWidth());
        CHECK_EQUAL(h, js->getImageHeight());
        CHECK_EQUAL(ssp, js->getSamplesPerPixel());
    

        delete js;
    }

}

#endif //TEST_JITTERED_SAMPLER_H
