#ifndef DYNAMIC_SPECTRUM_H
#define DYNAMIC_SPECTRUM_H

#include <vector>
#include <string>
#include <ostream>

/**
 *  Class for representing point sampled spectra.  Only
 *  works with a uniform sampling pattern.
 */
class DynamicSpectrum{
public:

    /// Items from this enum can be passed into a c-tor to create a spectrum that
    /// matches a CIE reference illuminant.
    enum CIEReferenceIlluminant{
        CIE_ILLUM_A    , //Tungsten incandescent bulb

        //NOTE: Illuminants D50,55, and 75 are not included as I can not find tabulated data
        //CIE_ILLUM_C    , //North-sky daylight (obsolete)
        //for them.  TODO: Fix this.
        //CIE_ILLUM_D_50 , //Horizon daylight
        //CIE_ILLUM_D_55 , //Mid-morning or mid-afternoon light
        CIE_ILLUM_D_65 , //Noon daylight (preferred over illum B)
        //CIE_ILLUM_D_75 , //North Sky daylight (preferred over illum C)

        CIE_ILLUM_E    , //Equal enerey (constant 100)

        //F_1, ... ,F_12 = various flourescents
        CIE_ILLUM_F_1, CIE_ILLUM_F_2, CIE_ILLUM_F_3, CIE_ILLUM_F_4,
        CIE_ILLUM_F_5, CIE_ILLUM_F_6, CIE_ILLUM_F_7, CIE_ILLUM_F_8,
        CIE_ILLUM_F_9, CIE_ILLUM_F_10, CIE_ILLUM_F_11, CIE_ILLUM_F_12
    };

    /// Items from this enum can be passed into a c-tor to create a spectrum
    /// that matches a particular spectral matching curve.
    /// Xbar, Ybay, and Zbar functions from the CIE
    /// for various viewing angles.
    /// Also the CIE reccomended luminous effeciency function in included for both
    /// photopic("normal vision") and scoptic("low light") vision.
    enum SpectralMatchingCurve{
        CIE_X_2_DEG,
        CIE_Y_2_DEG,
        CIE_Z_2_DEG,
        CIE_X_10_DEG,
        CIE_Y_10_DEG,
        CIE_Z_10_DEG,
        CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951,
        CIE_LUMINOUS_EFFICIENCY_SCOPTIC_1951
    };

    /**
     *  Default constructor.  Creates a spectrum with a value of 1 over [400,720].
     */
    DynamicSpectrum(void);

    /**
     *  Construct a spectrum from a CIE standard reference illuminant.
     */
    DynamicSpectrum(CIEReferenceIlluminant illum);

    /**
     *  Construct a spectrum from a spectral matching curve(CMF).
     */
    DynamicSpectrum(SpectralMatchingCurve sCurve);

    /**
     *  Construct a spectrum from a vector of floats.
     *  @param values = Each of the spectral samples.
     *  @param firstSampleNm = Wavelength (nm) of the first sample.
     *  @param sampleStep = Wavelength of second sample - wavlength of first sample.
     */
    DynamicSpectrum(const std::vector<float>& values,
        float firstSampleNm, float sampleStep);

    /**
     *  Load a spectrum from an array.  The array is copied, not shared.
     *
     *  @param vals is the array of spectral samples.
     *  @param minNmVal is the wavelength of the first sample.
     *  @param maxNmVal is the wavelength of the last sample.
     *  @param stepNm is the distance in nm between samples.
     */
    DynamicSpectrum(float* vals, float minNmVal,
        float maxNmVal, float stepNmVal, int numSamps);


    /**
     *  Construct a spectrum with a constant value.
     *  @param val is the value
     *  @param nmMin is the wavelength of the first sample.
     *  @param nmMax is the wavelength of the last sample.
     */
    DynamicSpectrum(float val, float minnm, float maxnm);

    /**
     *  Construct a blackbody radiator via
     *  Plank's law.
     *
     *  @param Tkelvin is the temperature of the blackbody in kelvin.
     *  @param minNmVal is the minimum wavelength(nm) over which to define
     *   the spectrum.
     *  @param maxNmVal is the max wavelength(nm) over which to define
     *   the spectrum.
     *  @param numSamples is the number of samples in the range [minNmVal, maxNmVal]
     *   Tthis includes the two samples on the end.
     */
    DynamicSpectrum(float Tkelvin, float minNmVal, float maxNmVal, int numSamples);

    /**
     *  Typedef spectral_eval_function corresponds to a function pointer for a function
     *  that takes a float and returns a float.  This function can be used to encode a spectrum
     *  as a function.
     */
    typedef float (*spectral_eval_function)(float);
    DynamicSpectrum(spectral_eval_function seval, float minNmVal, float maxNmVal, float stepVal);

    /**
     *  Copy constructor.  Deep copy.
     */
    DynamicSpectrum(const DynamicSpectrum& other);

    /**
     *  Assignment operator.  Deep copy.
     */
    DynamicSpectrum& operator=(const DynamicSpectrum& rhs);

    /**
     *  Get a spectrum from disk. The ok parameter is set to true on success, failse
     *  on failure.
     */
    static DynamicSpectrum fromDisk(const std::string& path, bool& ok);

    /**
     *  Write a spectrum to disk.  Return true on success.
     */
    bool toDisk(const std::string& path)const;

    /**
     *  Destructor.
     */
    virtual ~DynamicSpectrum();

    //Conversions to tristimulus color models
    virtual void toRGB(float& r, float& g, float& b, float K)const;
    virtual void toXYZ(float& x, float& y, float& z, float K)const;

    /**
     *  Find the p-norm of a DynamicSpectrum, where the samples are interpreted as a vector.
     *  @param normP corresponds to the P in L-p norm.
     *  @return the norm.
     *  @precondition normP must be > 0
     */
    float norm(int normP)const;

    //Functions for getting information about the sampling
    virtual float getMinNm()const;
    virtual float getMaxNm()const;
    virtual std::vector<float> getSampleNms()const;
    virtual int getNumSamples()const;
    virtual float getSampleStep()const;

    //Functions for sampling the spectrum

    /// \brief Sample the idx'th sample.
    ///  No bounds checking is performed.
    virtual float operator[](int idx)const;

    /// \brief Sample with linear interpolation between samples.
    ///  0 is returned for out of range values.
    virtual float operator()(float nm)const;


    virtual float max()const;
    virtual float min()const;

    //Setters
    void setSample(int idx, float val);

    bool operator==(const DynamicSpectrum& other)const;
    bool operator!=(const DynamicSpectrum& other)const;

    //Operators
    //DynamicSpectrum OP scalar operators
    DynamicSpectrum operator*(float scalar)const;
    DynamicSpectrum operator/(float scalar)const;
    DynamicSpectrum operator+(float scalar)const;
    DynamicSpectrum operator-(float scalar)const;

    //DynamicSpectrum OP spectrum
    DynamicSpectrum operator*(const DynamicSpectrum& rhs)const;
    DynamicSpectrum operator/(const DynamicSpectrum& rhs)const;
    DynamicSpectrum operator+(const DynamicSpectrum& rhs)const;
    DynamicSpectrum operator-(const DynamicSpectrum& rhs)const;

    //Definite integrals (area under curve)
    //TODO: Include other integrators

    //Integrate via trapezoidal rule
    float integrate()const;
    float defIntegralTrapezoid(float nmBegin, float nmEnd)const;

    //Overloaded stream insertion
    friend std::ostream& operator<<(std::ostream& os, const DynamicSpectrum& s);
private:
    int N;
    float* data;
    float minNm, maxNm, stepNm;
    void assertInvariant()const; //Used only in debug mode
};


#endif //DYNAMIC_SPECTRUM_H
