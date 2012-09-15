#ifndef REGULARLY_SAMPLED_SPECTRUM_H
#define REGULARLY_SAMPLED_SPECTRUM_H

#include <vector>
//--
#include "utils/Assert.h"
#include "utils/Constants.h"
#include "math/Interpolation.h"

/**
 *  Spectrum class.  This class represents a spectrum as a set of evenly
 *  spaced point samples over the interval getStartWavelength() to getEndWavelength().
 *  This interval is currently [380nm, 780nm], which represents the wavelength range over
 *  which the human eye is sensitive (the "VIS" range).
 */
class RegularlySampledSpectrum{
public:
    static const size_t MAX_SPECTRAL_SAMPLES = 80;
    static const size_t MIN_SPECTRAL_SAMPLES = 4;
    inline static float getStartWavelength(){ return 380.0f; }
    inline static float getEndWavelength()  { return 780.0f; }

    /**
     *  Construct a point-sampled spectrum with the same value all the way across VIS.
     *  @param nSamples is the desired number of spectral samples.
     *  @param constantVal is the value all the way across VIS.  Defaults to 0.
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
     *  @param nSamples is the desired number of point samples.
     */
    RegularlySampledSpectrum(const std::vector<float>& wavelengths,
        const std::vector<float>& sampleVals, const size_t nSamples);

    //TODO(Colin): Include a constructor for initializing a spectrum with regularly sampled data...
    //TODO(Colin): Include a constructor for initializing common spectral data

    /// \brief Copy constructor.
    RegularlySampledSpectrum(const RegularlySampledSpectrum& other);

    /// \brief Assignment operator.
    RegularlySampledSpectrum& operator=(const RegularlySampledSpectrum& rhs)const;

    /// \brief Get the number of point samples.
    size_t getNumSamples()const;

    /// \brief Get the spacing(in nanometers) between two adjacent point samples.
    float getStepNm()const;

    /**
     *  Get the wavelength(in nanometers) of the idx-th point sample.
     *  @param idx is a 0 based index, should be in the range [0, getNumSamples())
     *  or this will crash.
     *  @return the value of sample[idx].
     */
    float getSampleNm(size_t idx)const;

    /**
     *  Set the value of the of the idx-th point sample.
     *  @param idx is a 0 based index, should be in the range [0, getNumSamples())
     *  or this will crash.
     *  @param val is the value the idx-th sample will be set to.  Must be >= 0.0f.
     */
    void setSample(size_t idx, float val);

    /**
     * Get the value fo the idx-th sample.
     * @param idx is a 0 based index, should be in the range [0, getNumSamples())
     * or this will crash.
     * @return the value of sample[idx]
     */
    float getSample(size_t idx)const;

    /// \brief Syntactic sugar for getSample(idx).
    float operator[](size_t idx)const;

   /**
    * Sample the spectrum at an arbitrary  wavelength value(in nanometers)
    * Samples are linearly interpolated between, and samples outside of the visible
    * range will always return 0.0f.
    * For example:
    *  <pre>
    *  {@code
    *  RegularlySampledSpectrum spec(...);
    *  float val_at_512nm = spec(512.0f);
    *  }
    *  </pre>
    *  @param nm is the wavelength at which to sample.
    *  @return a linearly interpolated value.  Returns the *exact* value
    *  getSample(i) iff nm == getSampleNm(i).
    */
    float operator()(const float nm)const;

   /**
    *  Find the smallest and largest wavelengths with corresponding
    *  non-zero samples.  For example, if our spectrum was this:
    *  <pre>
    *   f =
    *    s(380) = 0
    *    s(420) = 1.1
    *    s(460) = 0
    *    s(500) = 1.2
    *    s(540) = 3
    *    s(580) = 4
    *    s(600) = 5
    *    s(620) = 4.1
    *    s(660) = 3
    *    s(700) = 1.4f
    *    s(740) = 0
    *    s(780) = 0
    *  Then:
    *  {@code
    *  float begin, end; begin = end = 0.0f;
    *  bool ok = getNonZeroExtent(begin,end);
    *  @code}
    *  begin is now 420.0f
    *  end is now 700.0f
    *  </pre>
    *
    *  @param posBeginNm is the return parameter for the first non-zero
    *  sample wavength.
    *  @param posEndNm is the return parameter for the last non-zero
    *  sample wavength.
    *  @return true if the spectrum is non-zero over any extent.
    */
    bool getNonZeroExtent(float& posBeginNm, float& posEndNm)const;

    /// \brief Get the value of the largest sample.
    float getMaxValue()const;

    /// \brief Get the value of the smallest sample.
    float getMinValue()const;


    //TODO(Colin): Overload common operators






    float integrate1()const;
    float integrate2()const;
    float integrate3()const;
    float integrate4()const;
    float integrate5()const;
    float integrate6()const;
    float integrate7()const;

private:
    size_t N; //TODO: Maybe store as a float too?
    float data[MAX_SPECTRAL_SAMPLES];

    /// \brief Return true iff all samples are >= 0.0f and N is within
    /// [MIN_SPECTRAL_SAMPLES, MAX_SPECTRAL_SAMPLES].
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

inline float RegularlySampledSpectrum::operator[](size_t idx)const{
    return getSample(idx);
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
    temp_1 = temp_2 = 0.0f;
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
