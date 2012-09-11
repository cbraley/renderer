#ifndef REGULARLY_SAMPLED_SPECTRUM_H
#define REGULARLY_SAMPLED_SPECTRUM_H

#include <vector>
//--
#include "utils/Assert.h"
#include "utils/Constants.h"
#include "math/Interpolation.h"

/**
 *  Spectrum class.  Point sampled regularly over the interval
 *  [380, 780].  Templated on the number of samples.
 *
 */
class RegularlySampledSpectrum{
public:
    static const size_t MAX_SPECTRAL_SAMPLES = 80;
    static const size_t MIN_SPECTRAL_SAMPLES = 4;
    inline static float getStartWavelength(){ return 380.0f; }
    inline static float getEndWavelength()  { return 780.0f; }

    /**
     *  Construct a spectrum with the same value all the way across VIS.
     *  @param constantVal is the value all the way across VIS.  Defauls to 0.
     */
    RegularlySampledSpectrum(size_t nSamples, float constantVal = 0.0f);

    /**
     *  Construct a spectrum from a potentially unsorted and irregularly spaced set of
     *  spectral samples.  wavelengths[i] should be the wavelength, in nanometers, of
     *  sampleVals[i].  The size of wavelengths must match the size of sampleVals.
     *  Note that these irregular samples will be linearly interpolated between and will
     *  be resampled with regular spacing.
     *
     *  @param wavelengths a list of sample wavelengths in nm.  May be unsorted.
     *  @param sampleVals a list of positive floating point numbers; must have the same
     *  size as wavelengths.
     */
    RegularlySampledSpectrum(const std::vector<float>& wavelengths,
        const std::vector<float>& sampleVals, const size_t nSamples);

    /**
     * \brief Copy constructor.
     */
    RegularlySampledSpectrum(const RegularlySampledSpectrum& other);

    /**
     *  \brief Assignment operator.
     */
    RegularlySampledSpectrum& operator=(const RegularlySampledSpectrum& rhs)const;

    /**
     * \brief Get the number of point samples.
     */
    size_t getNumSamples()const;

    float getStepNm()const;

    float getSampleNm(size_t idx)const;
    void setSample(size_t idx, float val);
    float getSample(size_t idx)const;

    float operator()(const float nm)const;
    float operator[](size_t index)const;

    bool getExtent(float& posBeginNm, float& posEndNm)const;

    float getMaxValue()const;
    float getMinValue()const;

    float integrate1()const;
    float integrate2()const;
    float integrate3()const;
    float integrate4()const;
    float integrate5()const;
    float integrate6()const;

private:
    size_t N; //TODO: Maybe store as a float too?
    float data[MAX_SPECTRAL_SAMPLES];
    bool dataIsValid()const;
};

//Inline function definitions -------------------------------------------------
inline size_t RegularlySampledSpectrum::getNumSamples()const{ return N; }

inline float RegularlySampledSpectrum::getStepNm()const{
    const float step = (getEndWavelength() - getStartWavelength()) / 
        static_cast<float>(N-1);
    return step;
}

inline float RegularlySampledSpectrum::getSampleNm(size_t idx)const{
    Assert(idx >= 0 && idx < N);
    return getStartWavelength() +
        getStepNm() * static_cast<float>(idx);
}

inline void RegularlySampledSpectrum::setSample(size_t idx, float val){
    Assert(idx >= 0 && idx < N);
    Assert(val >= 0.0f);
    data[idx] = val;
}

inline float RegularlySampledSpectrum::getSample(size_t idx)const{
    Assert(idx >= 0 && idx < N);
    return data[idx];
}

inline float RegularlySampledSpectrum::operator()(const float nm)const{
    const float step = getStepNm();
    const int indexBefore = static_cast<int>((nm - getStartWavelength())/step);
    const int indexAfter = indexBefore + 1;

    return (indexBefore < 0 || indexAfter >= static_cast<int>(N)) ? 0.0f : 
        Interpolation::lerp<float>(
        nm,
        getStartWavelength() + (step * static_cast<float>(indexBefore)),
        getStartWavelength() + (step * static_cast<float>(indexAfter )),
        data[indexBefore],
        data[indexAfter]
        );
}

inline float RegularlySampledSpectrum::operator[](size_t index)const{
    return getSample(index);
}

inline float RegularlySampledSpectrum::getMaxValue()const{
    float maxVal = data[0];
    for(size_t i = 1; i < N; ++i){
        maxVal = std::max<float>(maxVal, data[i]); 
    }
    return maxVal;
}
inline float RegularlySampledSpectrum::getMinValue()const{
    float minVal = data[0];
    for(size_t i = 1; i < N; ++i){
        minVal = std::min<float>(minVal, data[i]); 
    }
    return minVal;
}

inline float RegularlySampledSpectrum::integrate1()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    for(size_t i = 0; i < N; ++i){
        sum += data[i];
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate2()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    for(size_t i = 0; i < MAX_SPECTRAL_SAMPLES; ++i){
        sum += data[i];
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate3()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    float* temp = const_cast<float*>(data);
    float* end = const_cast<float*>(data) + MAX_SPECTRAL_SAMPLES;
    while(temp != end){
        sum += *temp++;
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate4()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    float* temp = const_cast<float*>(data);
    float* end = const_cast<float*>(data) + MAX_SPECTRAL_SAMPLES;
    while(temp != end){
        sum += *temp++;
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate5()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    float* temp = const_cast<float*>(data);
    float* end = const_cast<float*>(data) + MAX_SPECTRAL_SAMPLES;
    while(temp != end){
        sum += *temp++;
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate6()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    float* temp = const_cast<float*>(data);
    float* end = const_cast<float*>(data) + N;
    Assert(N % 8 == 0);
    while(temp != end){
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
        sum += *temp++;
    }
    return sum * mult;
}

inline float RegularlySampledSpectrum::integrate7()const{
    const float trapWidth = getStepNm(); //width of trapezoid
    const float mult = 0.5f * trapWidth; //.5 * trapWidth(h_1 + h_2)
    float sum = 0.0f;
    float* temp = const_cast<float*>(data);
    float* end = const_cast<float*>(data) + N;
    Assert(N % 8 == 0);
    float temp_1, temp_2;
    temp_1 = temp+2 = 0.0f;
    while(temp != end){
        temp_1 = temp_2 = 0.0f;

        temp_1 += *temp++;
        temp_1 += *temp++;
        temp_1 += *temp++;
        temp_1 += *temp++;
        
        temp_2 += *temp++;
        temp_2 += *temp++;
        temp_2 += *temp++;
        temp_2 += *temp++;

        sum += temp_1 + temp_2;

    }
    return sum * mult;
}




#endif //REGULARLY_SAMPLED_SPECTRUM_H
