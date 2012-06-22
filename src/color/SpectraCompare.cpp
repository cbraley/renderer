#include "color/SpectraCompare.h"

#include <algorithm>
#include <cmath>
//--
#include "utils/Assert.h"

//All comparision metrics that require discrete sampling must have some
//sampling interval.  It is SAMPLE_STEP.
static const float SAMPLE_STEP = 1.0f;

float SpectraCompare::specCompareRMS(const Spectrum* a, const Spectrum* b){

    //Compare over whatever range we have any spectral data
    const float beginComp = std::min<float>(a->getMinNm(), b->getMinNm());
    const float endComp   = std::max<float>(b->getMaxNm(), b->getMaxNm());
    Assert(endComp >= beginComp);


    //Total number of comparision samples.
    //The +1 is to account for situations like:
    //   endComp = 20, beginComp = 10, SAMPLE_STEP = 5
    //   since in this situation we expect 3 samples:
    //   10,15,20
    const int N = ((endComp - beginComp) / SAMPLE_STEP) + 1;

    float accum = 0.0f;
    for(int i = 0; i < N; i++){
        const float nm = SAMPLE_STEP * i;
        const float diff = (*a)(nm) - (*b)(nm);
        accum += diff * diff;
    }

    //Note this 1.0f - ... is here since we have decided that
    //1.0 indicates a perfect match, whereas 0.0f indicates a
    //terrible match
    const float ret = 1.0f - sqrtf(accum / N);
    Assert(ret >= 0.0f && ret <= 1.0f);
    return ret;
}

float SpectraCompare::specCompareGFC(const Spectrum* a, const Spectrum* b){

    //Compare over whatever range we have any spectral data
    const float beginComp = std::min<float>(a->getMinNm(), b->getMinNm());
    const float endComp   = std::max<float>(b->getMaxNm(), b->getMaxNm());
    Assert(endComp >= beginComp);

    //Total number of comparision samples.
    //The +1 is to account for situations like:
    //   endComp = 20, beginComp = 10, SAMPLE_STEP = 5
    //   since in this situation we expect 3 samples:
    //   10,15,20
    const int N = ((endComp - beginComp) / SAMPLE_STEP) + 1;

    float accumNum  = 0.0f; //numerator
    float accumDenL = 0.0f; //denomintor left
    float accumDenR = 0.0f; //denomintor right

    for(int i = 0; i < N; i++){
        const float nm = SAMPLE_STEP * i;
        const float aSamp = (*a)(nm);
        const float bSamp = (*b)(nm);

        accumNum  += aSamp * bSamp;
        accumDenL += aSamp * aSamp;
        accumDenR += bSamp * bSamp;
    }

    const float denominator = sqrtf(fabs(accumDenL)) * sqrtf(fabs(accumDenR));
    const float ret = fabs(accumNum) / (denominator);
    Assert(ret >= 0.0f && ret <= 1.0f);
    return ret;
}

