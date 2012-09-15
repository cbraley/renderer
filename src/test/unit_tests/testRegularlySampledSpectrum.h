#ifndef TEST_REGULARLY_SAMPLED_SPECTRUM_H
#define TEST_REGULARLY_SAMPLED_SPECTRUM_H

#include <algorithm>
#include <vector>
//--
#include <UnitTest++.h>
//--
#include "color/RegularlySampledSpectrum.h"
#include "math/MathUtils.h"

SUITE(RegularlySampledSpectrumTests){

    inline float randomFloat(float a, float b){
        return ((b-a)*((float)rand()/RAND_MAX))+a;
    }

    
    TEST(TestNonzeroExtentComputation){

        // Construct a constant spectrum, make sure its extent is correct
        RegularlySampledSpectrum spec(20, 2.0f);
        float minExtent, maxExtent; minExtent = maxExtent = 0.0f;
        CHECK(spec.getNonZeroExtent(minExtent, maxExtent));
        CHECK_EQUAL(RegularlySampledSpectrum::getStartWavelength(), 
            minExtent);
        CHECK_EQUAL(RegularlySampledSpectrum::getEndWavelength(), 
            maxExtent);

    }

    

}

#endif //TEST_REGULARLY_SAMPLED_SPECTRUM_H
