#include "color/RegularlySampledSpectrum.h"

#include <cstring>
//--
#include "math/MathUtils.h"
#include "math/IrregularlySampledFunction.h"


bool RegularlySampledSpectrum::dataIsValid()const{
    for(size_t i = 0; i < MAX_SPECTRAL_SAMPLES; ++i){
        const bool sampleOK = data[i] >= 0.0f &&
            !MathUtils::isNaN(data[i]);
        if(!sampleOK){
            return false;
        }
    }
    return getStartWavelength() < getEndWavelength();
}

RegularlySampledSpectrum::RegularlySampledSpectrum(size_t nSamples,
    float constantVal) : N(nSamples) {
    Assert(N >= MIN_SPECTRAL_SAMPLES && N <= MAX_SPECTRAL_SAMPLES);
    for(size_t i = 0; i < N; ++i){
        data[i] = constantVal;
    }
    // Data beyond N is 0'ed out so that the loops for computing definite
    //integrals can be unrolled by the compiler.  MAX_SPECTRAL_SAMPLES is known
    //at compile time, and therefore loops from i = 0  to MAX_SPECTRAL_SAMPLES
    //are auto-unrolled, whereas loops from i = 0...N may have issues being fully unrolled.
    //TODO: Verify by looking at asm
    for(size_t i = N; i < MAX_SPECTRAL_SAMPLES; ++i){
        data[i] = 0.0f;
    }
}


RegularlySampledSpectrum::RegularlySampledSpectrum(const std::vector<float>& wavelengths,
    const std::vector<float>& sampleVals, const size_t nSamples) : N(nSamples){
    
    Assert(N >= MIN_SPECTRAL_SAMPLES && N <= MAX_SPECTRAL_SAMPLES);
    Assert(wavelengths.size() == sampleVals.size());

    // Make the function and sample it
    IrregularlySampledFunction func(wavelengths, sampleVals);
    for(size_t i = 0; i < N; ++i){
        const float nm = getSampleNm(i);
        data[i] = func(nm);
    }
    Assert(dataIsValid());
}



bool RegularlySampledSpectrum::getNonZeroExtent(float& posBeginNm, float& posEndNm)const{
    posBeginNm = posEndNm = -999.0f;

    //TODO: Could be optimized...
    size_t i = 0;
    for(; i < N; ++i){
        if(data[i] > 0.0f){
            posBeginNm = getSampleNm(i);
            break;
        }
    }
    size_t j = N-1;
    for(; j >= i; --j){
        if(data[j] > 0.0f){
            posEndNm = getSampleNm(j);
            break;
        }
    }

    return posBeginNm <= posEndNm && 
        posBeginNm > 0.0f && posEndNm > 0.0f;
}

