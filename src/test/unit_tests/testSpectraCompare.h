#ifndef TEST_SPECTRA_COMPARE
#define TEST_SPECTRA_COMPARE

#include <UnitTest++.h>
//--
#include "color/SpectraCompare.h"
#include <cmath>
#include "color/Spectrum.h"

SUITE(SpectraCompareTests){

    TEST(SpectraCompareSanity){

        const float EPS_VAL = 0.00001f;

        Spectrum X(Spectrum::CIE_X_2_DEG);
        Spectrum Y(Spectrum::CIE_Y_2_DEG);
        Spectrum Z(Spectrum::CIE_Z_2_DEG);

        float gfcXY = SpectraCompare::specCompareGFC(&X,&Y);
        float gfcYX = SpectraCompare::specCompareGFC(&Y,&X);
        float gfcXX = SpectraCompare::specCompareGFC(&X,&X);

        float rmsXY = SpectraCompare::specCompareRMS(&X,&Y);
        float rmsYX = SpectraCompare::specCompareRMS(&Y,&X);
        float rmsXX = SpectraCompare::specCompareRMS(&X,&X);


        CHECK_CLOSE(gfcXX, 1.0f, EPS_VAL); //Should agree with itself
        CHECK_CLOSE(fabs(gfcXY - gfcYX), 0.0f, EPS_VAL); //Argument order should not matter

        CHECK_CLOSE(rmsXX, 1.0f, EPS_VAL); //Should agree with itself
        CHECK_CLOSE(fabs(rmsXY - rmsYX), 0.0f, EPS_VAL); //Argument order should not matter
    }

}

#endif //TEST_SPECTRA_COMPARE
