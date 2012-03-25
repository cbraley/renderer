#ifndef TEST_JITTERED_SAMPLER_H
#define TEST_JITTERED_SAMPLER_H

#include <UnitTest++.h>
//--
#include "JitteredSampler.h"
#include "ImageSampler.h"

SUITE(JitteredSamplerTests){

    TEST(Basics){
        const int NUM_TRIALS = 32;

        int w = 101;
        int h = 102;
        int ssp = 10;
        float jitX = 2.0f;
        float jitY = -3.0f;
        ImageSampler* js = new JitteredSampler(w,h,ssp, jitX, jitY);

        //Check the basics
        CHECK_EQUAL(w, js->getImageWidth());
        CHECK_EQUAL(h, js->getImageHeight());
        CHECK_EQUAL(ssp, js->getSamplesPerPixel());
    
        //Run trials and make sure we always stayed in the bounds
        for(int trial = 0; trial < NUM_TRIALS; trial++){
            int left   = 0;
            int top    = 0;
            int bottom = h-1;
            int right  = w-1;

            ImageSampler::Sample* arr = js->getSamplesForSubwindow(left, right, top, bottom);
            const int sz = js->getNumSamplesForWindow(left, right, top, bottom);
            for(int i = 0; i < sz; i++){
                ImageSampler::Sample curr = arr[i];
                //TODO: What checks can we do here?
            }

            delete[] arr;
        }



        delete js;
    }

}

#endif //TEST_JITTERED_SAMPLER_H
