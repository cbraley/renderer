#include "color/Spectrum.h"

#include <cstring>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <utility>
#include <ctime>
//--
#include "utils/Assert.h"
#include "color/LightData.h"
#include "color/ColorData.h" //Spectral reference data
#include "color/ColorUtils.h"
#include "utils/StringUtils.h"

void Spectrum::assertInvariant()const{
    Assert(N > 0);
    Assert(data != NULL);

    if(N == 1){ //Special case for 1 sample
        Assert(minNm == maxNm);
        return;
    }

    Assert(maxNm > minNm);
    Assert(maxNm - minNm >= stepNm);
//    Assert( int((maxNm - minNm)/stepNm) == N-1);
}


Spectrum::Spectrum(void) :
    N(2), data(NULL), minNm(400.0f), maxNm(720.0f), stepNm(320.0f)
{
    data = new float[2];
    data[0] = data[1] = 1.0f;
    assertInvariant();
}

Spectrum::Spectrum(CIEReferenceIlluminant illum){

    ILLUMINANT_SERIES series;
    int illumIndex = -1;

    if(illum == CIE_ILLUM_E){ //Equal energy 100 across VIS
        series = E; illumIndex = -1;
    }else if( illum == CIE_ILLUM_A    ){ //Tungsten incandescent bulb
        series = A; illumIndex = -1;
    /*
    }else if( illum == CIE_ILLUM_C    ){ //North-sky daylight (obsolete)
        series = C; illumIndex = -1;
    }else if( illum == CIE_ILLUM_D_50 ){ //Horizon daylight
        series = D;
        illumIndex = 50;
    }else if( illum == CIE_ILLUM_D_55 ){ //Mid-morning or mid-afternoon light
        series = D;
        illumIndex = 55;
    */
    }else if( illum == CIE_ILLUM_D_65 ){ //Noon daylight (preferred over illum B)
        series = D;
        illumIndex = 65;
    /*
    }else if( illum == CIE_ILLUM_D_75 ){ //North Sky daylight (preferred over illum C)
        series = D;
        illumIndex = 75;
    */
    }else if(illum == CIE_ILLUM_F_1){
        series = F;
        illumIndex = 1;
    }else if(illum == CIE_ILLUM_F_2){
        series = F;
        illumIndex = 2;
    }else if(illum == CIE_ILLUM_F_3){
        series = F;
        illumIndex = 3;
    }else if(illum == CIE_ILLUM_F_4){
        series = F;
        illumIndex = 4;
    }else if(illum == CIE_ILLUM_F_5){
        series = F;
        illumIndex = 5;
    }else if(illum == CIE_ILLUM_F_6){
        series = F;
        illumIndex = 6;
    }else if(illum == CIE_ILLUM_F_7){
        series = F;
        illumIndex = 7;
    }else if(illum == CIE_ILLUM_F_8){
        series = F;
        illumIndex = 8;
    }else if(illum == CIE_ILLUM_F_9){
        series = F;
        illumIndex = 9;
    }else if(illum == CIE_ILLUM_F_10){
        series = F;
        illumIndex = 10;
    }else if(illum == CIE_ILLUM_F_11){
        series = F;
        illumIndex = 11;
    }else if(illum == CIE_ILLUM_F_12){
        series = F;
        illumIndex = 12;
    }else{
        Assert(false);
    }
    //TODO: Implement the rest of the lamps

    //Load it
    Illuminant ill = getIlluminant(series, illumIndex);
    N = ill.N;
    data = ill.data;
    minNm = ill.nmMin;
    maxNm = ill.nmMax;
    stepNm = ill.nmStep;

    assertInvariant();
}

Spectrum::Spectrum(SpectralMatchingCurve sCurve) :
    N(-1), data(NULL), minNm(-1.0f),
    maxNm(-1.0f), stepNm(-1.0f)
{

    //We declare an associative array of enums to enums to avoid a giant
    //case statement
    typedef std::pair<SpectralMatchingCurve, CMF_ID> EnumMapHelper;
    const int NUM_DATA = 8;
    EnumMapHelper dict[NUM_DATA] = {
        std::make_pair(CIE_X_2_DEG, X_CMF_2DEG),
        std::make_pair(CIE_Y_2_DEG, Y_CMF_2DEG),
        std::make_pair(CIE_Z_2_DEG, Z_CMF_2DEG),
        std::make_pair(CIE_X_10_DEG, X_CMF_10DEG),
        std::make_pair(CIE_Y_10_DEG, Y_CMF_10DEG),
        std::make_pair(CIE_Z_10_DEG, Z_CMF_10DEG),
        std::make_pair(CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951, PHOTOPIC),
        std::make_pair(CIE_LUMINOUS_EFFICIENCY_SCOPTIC_1951, SCOPTIC)
    };

    //Find our mapping
    int idx = 0;
    bool found = false;
    for(idx = 0; idx < NUM_DATA; idx++){
        if(dict[idx].first == sCurve){
            found = true;
            break;
        }
    }
    Assert(found);

    //Load the CMF
    CMF cmf = getCMF(dict[idx].second);

    //Copy in the data
    N = cmf.N;
    data = cmf.data;
    minNm = cmf.nmMin;
    maxNm = cmf.nmMax;
    stepNm = cmf.nmStep;

    assertInvariant();
}



Spectrum::Spectrum(const std::vector<float>& values, float firstSampleNm,
    float sampleStep) : N(values.size()), data(NULL), minNm(firstSampleNm),
    maxNm(firstSampleNm + float(values.size()-1) * sampleStep ),
    stepNm(sampleStep)
{
    Assert(N > 0);
    Assert(stepNm > 0.0f);
    Assert(minNm <= maxNm);

    data = new float[N];
    for(size_t i = 0; i < values.size(); i++){
        data[i] = values[i];
    }
    assertInvariant();
}

Spectrum::Spectrum(float* vals, float minNmVal, float maxNmVal,
    float stepNmVal, int numSamps) :
    N(numSamps), data(NULL), minNm(minNmVal), maxNm(maxNmVal), stepNm(stepNmVal)
{
    Assert(vals != NULL);
    data = new float[N];
    memcpy((void*)data, (void*)vals, N * sizeof(float));
    assertInvariant();
}

//Helper function for Planck's law of blackbody radiation
inline static float planck(const float lambda, const float T){
    const float hc  = 1.986446e-16; //Planck's constant times the speed of light
    const float hcc = hc * 2.998e17;
    const float kB  = 1.38065e-23; //Boltzman constant
    const float exponent = (hc) / (lambda * kB * T);

    const float term1 = ((2.0f * hcc)/(pow(lambda,5)));
    const float term2 = 1.0f / (exp(exponent) -1.0f);
    return term1 * term2 * 1e13; //TODO: The 1e13 is a hack to put these spectra into a decent numeric range
}


Spectrum::Spectrum(float Tkelvin, float minNmVal, float maxNmVal, int numSamples) :
    N(numSamples), data(NULL), minNm(minNmVal), maxNm(maxNmVal),
    stepNm((maxNmVal - minNmVal)/float(numSamples-1))
{
    Assert(Tkelvin > 0);
    Assert(minNmVal <= maxNmVal);
    Assert(numSamples > 1 || minNmVal || maxNmVal);

    if(minNmVal == maxNmVal && numSamples == 1){
        //Delta spectrum case
        N = 1;
        minNm = maxNm = minNmVal;
        stepNm = 0.0f;
        data = new float[1];
        data[0] = planck(minNm, Tkelvin);
    }else{ //Normal case
        data = new float[N];
        float tempNm = minNmVal;
        for(int i = 0; i < N; i++){
            data[i] = planck(tempNm, Tkelvin);
            tempNm += stepNm;
        }
    }
    assertInvariant();
}


Spectrum::Spectrum(float val, float minnm, float maxnm) :
    N(2), minNm(minnm), maxNm(maxnm), stepNm(maxnm - minnm)
{
    data = new float[2];
    data[0] = data[1] = val;
    assertInvariant();
}

Spectrum::Spectrum(spectral_eval_function seval, float minNmVal,
    float maxNmVal, float stepVal) : N( int((maxNmVal - minNmVal)/stepVal) + 1 ),
    data(NULL), minNm(minNmVal), maxNm(maxNmVal), stepNm(stepVal)
{
    Assert(seval != NULL);
    Assert(minNm <= maxNm);
    Assert(N > 0);

    data = new float[N];
    float nm = minNm;
    for(int i = 0; i < N; i++){
        data[i] = (*seval)(nm);
        nm += stepNm;
    }
}

Spectrum Spectrum::fromDisk(const std::string& path, bool& ok){
    std::fstream fs(path.c_str(), std::ios::in);
    fs.precision( std::numeric_limits<double>::digits10 );
    const char COMMENT_CHAR = '#';
    if(! fs){
        fs.close();
        ok = false;
        return Spectrum();
    }

    int N = -1;
    float minWavelen = -1.0f;
    float maxWavelen = -1.0f;
    float stpWavelen = -1.0f;
    int index = 0;
    float* dataPtr = NULL;
    bool done = false;

    std::string line;
    do{
        std::getline(fs, line);

        //Skip blank and comment lines
        std::string trimmedLine = StringUtils::trim(line);
        if(trimmedLine.size() == 0 || trimmedLine[0] == COMMENT_CHAR){
            continue;
        }
        //Parse
        if(N == -1){
            N = atoi(trimmedLine.c_str());
            dataPtr = new float[N];
        }else if(minWavelen == -1.0f){
            minWavelen = atof(trimmedLine.c_str());
        }else if(maxWavelen == -1.0f){
            maxWavelen = atof(trimmedLine.c_str());
        }else if(stpWavelen == -1.0f){
            stpWavelen = atof(trimmedLine.c_str());
        }else if(index < N){
            dataPtr[index++] = atof(trimmedLine.c_str());
            done = index == N;
        }else{
            done = true;
            break;
        }
    }while(! fs.eof() );

    Spectrum ret(dataPtr, minWavelen, maxWavelen, stpWavelen, N);
    delete[] dataPtr;
    ok = done;
    return ret;
}

/**
    *  Write a spectrum to disk.  Return true on success.
    */
bool Spectrum::toDisk(const std::string& path)const{
    std::fstream fs(path.c_str(), std::ios::out);
    fs.precision( std::numeric_limits<double>::digits10 );
    if(! fs){
        fs.close();
        return false;
    }

    fs << "#Spectrum file from Colin Braley's Renderer" << std::endl;
    fs << "#Stores spectra with regularly spaced samples" << std::endl;
    time_t rawtime;
    time(&rawtime);
    char* x = ctime(&rawtime);
    fs << "#Written on: " << std::string(x) << std::endl;
    fs << std::endl;
    fs << "# Lines with the pound sign are comments, blank lines are OK." << std::endl;
    fs << "#File format is: " << std::endl;
    fs << "#N samples" << std::endl;
    fs << "#first wavelength" << std::endl;
    fs << "#last wavelength" << std::endl;
    fs << "#wavelength step" << std::endl;
    fs << std::endl;

    fs << N      << std::endl;
    fs << minNm  << std::endl;
    fs << maxNm  << std::endl;
    fs << stepNm << std::endl;
    fs << std::endl;
    for(int i = 0; i < N; i++){
        fs << data[i] << std::endl;
    }

    if(! fs){
        fs.close();
        return false;
    }
    fs.close();
    return true;
}


Spectrum::~Spectrum(){
    delete[] data; data = NULL;
}

void Spectrum::toRGB(float& r, float& g, float& b, float K)const{

    //First, convert to XYZ
    float X,Y,Z;
    X=Y=Z = 0.0f;
    toXYZ(X,Y,Z, K);

    //Now to RGB
    ColorUtils::XYZToRGB_sRGB(X,Y,Z, r,g,b);
}

void Spectrum::toXYZ(float& x, float& y, float& z, float K)const{

    //Load the XYZ curves
    Spectrum X(Spectrum::CIE_X_10_DEG);
    Spectrum Y(Spectrum::CIE_Y_10_DEG);
    Spectrum Z(Spectrum::CIE_Z_10_DEG);

    //Take the products
    Spectrum xProd = X * (*this);
    Spectrum yProd = Y * (*this);
    Spectrum zProd = Z * (*this);

    //Integrate area under curves, multiplying by K
    x = K * xProd.defIntegralTrapezoid(xProd.getMinNm(), xProd.getMaxNm());
    y = K * yProd.defIntegralTrapezoid(yProd.getMinNm(), yProd.getMaxNm());
    z = K * zProd.defIntegralTrapezoid(zProd.getMinNm(), zProd.getMaxNm());
}

float Spectrum::getMinNm()const{
    return minNm;
}
float Spectrum::getMaxNm()const{
    return maxNm;
}


std::vector<float> Spectrum::getSampleNms()const{
    std::vector<float> ret(N, 0.0f);
    float nm = getMinNm();
    const float nms = getSampleStep();
    for(int i = 0; i < N; i++){
        ret[i] = nm;
        nm += nms;
    }
    return ret;
}

int Spectrum::getNumSamples()const{
    return N;
}

float Spectrum::getSampleStep()const{
    return stepNm;
}

float Spectrum::operator[](int idx)const{
    return data[idx];
}

//#define DEBUG_PRINT
float Spectrum::operator()(float nm)const{


#ifdef DEBUG_PRINT
    std::cout << "operator( " << nm << ")" << std::endl;
#endif
    const float den = maxNm - minNm;
    if(den == 0.0f){ //Special case for spectra with a single sample (delta spectrum)
#ifdef DEBUG_PRINT
        std::cout << "\tSpecial case delta spectrum!" << std::endl;
#endif
        return nm == minNm ? data[0] : 0.0f;
    }
    float p = (nm - minNm) / den;
    if(p < 0.0f|| p > 1.0f){
#ifdef DEBUG_PRINT
        std::cout << "\tSpecial case OOB sample!" << std::endl;
#endif
        return 0.0f; //OOB
    }

    //If nm is in the range [minNm, maxNm] then p ranges
    //from [0,1] (inclusive) uniformly.  Note that p
    //may be out of [0,1] in the event that nm is not in the range
    //[minNm, maxNm]

    //Indices of values to sample (clamped to [0,1,...,N-1])

    int idxStopNoClamp = int(p * float(N - 1)) + 1;
    int idxStartNoClamp = idxStopNoClamp - 1;
    if(p == 1.0f){
        idxStopNoClamp  = N-1;
        idxStartNoClamp = N-2;
    }

    //const int idxStop = idxStopNoClamp >= N ? N-1 :
    //    (idxStopNoClamp < 0 ? 0 : idxStopNoClamp);
    //const int idxStartNoClamp = idxStop - 1;
    //const int idxStart = idxStartNoClamp < 0 ? 0 : idxStartNoClamp;

    //Return 0.0 for out of range values
    if(idxStopNoClamp >= N || idxStartNoClamp < 0){
#ifdef DEBUG_PRINT
        std::cout << "Out of bounds!" << std::endl;
        std::cout << "\t N = " << N << std::endl;
        std::cout << "\t minNm = " << minNm << std::endl;
        std::cout << "\t maxNm = " << maxNm << std::endl;
        std::cout << "\t---" << std::endl;
        std::cout << "\tidxStartNoClamp = " << idxStartNoClamp << std::endl;
        std::cout << "\tidxStopNoClamp = " << idxStopNoClamp << std::endl;
        std::cout << "\tp = " << p << std::endl;
#endif
        return 0.0f;
    }


    //Take samples
    Assert(idxStartNoClamp >= 0 && idxStartNoClamp < N);
    Assert(idxStopNoClamp >= 0 && idxStopNoClamp < N);
    const float valStart = data[idxStartNoClamp];
    const float valStop  = data[idxStopNoClamp];

    //Get nm values at end interpolation points
    const float nmStart = minNm + (float(idxStartNoClamp) * stepNm);
    const float nmStop  = minNm + (float(idxStopNoClamp ) * stepNm);

    //Now, as nm goes from [nmStart, nmStop]
    //return values from [valStart, valStop]
    const float pSmall = (nm - nmStart)/(nmStop - nmStart);


#ifdef DEBUG_PRINT
    std::cout << "\t N = " << N << std::endl;
    std::cout << "\t minNm = " << minNm << std::endl;
    std::cout << "\t maxNm = " << maxNm << std::endl;
    std::cout << "\t---" << std::endl;
    std::cout << "\tidxStartNoClamp = " << idxStartNoClamp << std::endl;
    std::cout << "\tidxStopNoClamp = " << idxStopNoClamp << std::endl;
    std::cout << "\tp = " << p << std::endl;
    std::cout << "\tnmStart = " << nmStart << std::endl;
    std::cout << "\tvalStart = " << valStart << std::endl;
    std::cout << "\tvalStop = " << valStop << std::endl;
    std::cout << "\tnmStop = " << nmStop << std::endl;
    std::cout << "\tpSmall = " << pSmall << std::endl;
#endif

    return valStart + (pSmall * (valStop - valStart));
}


float Spectrum::max()const{
    float c = std::numeric_limits<float>::min();
    for(int i = 0; i < N; i++){
        c = std::max<float>(c, data[i]);
    }
    return c;
}


float Spectrum::min()const{
    float c = std::numeric_limits<float>::max();
    for(int i = 0; i < N; i++){
        c = std::min<float>(c, data[i]);
    }
    return c;
}


void Spectrum::setSample(int idx, float val){
    Assert(idx >= 0 && idx < N);
    data[idx] = val;
}


bool Spectrum::operator==(const Spectrum& other)const{
    const float SPECTRA_CMP_TOL = 0.00001f;

    //See if the two spectra are so different we don't even need
    //to compare samples
    if(N != other.N || minNm != other.minNm || maxNm != other.maxNm ||
        stepNm != other.stepNm){


        std::cout << "Step A = " << stepNm       << std::endl;
        std::cout << "Step B = " << other.stepNm << std::endl;

        return false;
    }else{
        //Sample comparison was necessary
        for(int i = 0; i < N; i++){
            const float absDiff = fabsf(data[i] - other.data[i]);
            if(absDiff > SPECTRA_CMP_TOL){
                return false;
            }
        }
        return true;
    }
}

bool Spectrum::operator!=(const Spectrum& other)const{
    return !(*this == other);
}


float Spectrum::norm(int normP)const{
    Assert(normP > 0);

    float sum = 0.0f;
    if(normP == 2){ //L-2 norm (Euclidean)
        for(int i = 0; i < N; i++){
            sum += data[i] * data[i];
        }
        return sqrtf(sum);
    }else if(normP == 1){ //Taxicab norm
        for(int i = 0; i < N; i++){
            sum += fabsf(data[i]);
        }
        return sum;
    }else{ //General case
        for(int i = 0; i < N; i++){
            const float xi = fabsf(data[i]);
            sum += pow(xi, float(normP));
        }
        return pow(sum, 1.0f / float(normP));
    }
}


//Copy c-tor
Spectrum::Spectrum(const Spectrum& other) :
    N(other.N), data(NULL), minNm(other.minNm), maxNm(other.maxNm),
    stepNm(other.stepNm)
{
    delete[] data; data = NULL;
    data = new float[N];
    memcpy(data, other.data, N * sizeof(float));
}


Spectrum& Spectrum::operator=(const Spectrum& rhs){
    delete[] data; data = NULL;
    N = rhs.N;
    minNm = rhs.minNm;
    maxNm = rhs.maxNm;
    stepNm = rhs.stepNm;
    data = new float[N];
    memcpy(data, rhs.data, N * sizeof(float));
    return *this;
}

//Operators
Spectrum Spectrum::operator*(float scalar)const{
    Spectrum ret(*this);
    for(int i = 0; i < N; i++){
        ret.data[i] = data[i] * scalar;
    }
    return ret;
}


Spectrum Spectrum::operator/(float scalar)const{
    Spectrum ret(*this);
    for(int i = 0; i < N; i++){
        ret.data[i] = data[i] / scalar;
    }
    return ret;
}

Spectrum Spectrum::operator+(float scalar)const{
    Spectrum ret(*this);
    for(int i = 0; i < N; i++){
        ret.data[i] = data[i] + scalar;
    }
    return ret;
}


Spectrum Spectrum::operator-(float scalar)const{
    Spectrum ret(*this);
    for(int i = 0; i < N; i++){
        ret.data[i] = data[i] - scalar;
    }
    return ret;
}

Spectrum Spectrum::operator*(const Spectrum& rhs)const{
    float stepNmMin = std::min<float>(stepNm, rhs.stepNm);
    float beginMin  = std::min<float>(minNm, rhs.minNm);
    float endMax    = std::max<float>(maxNm, rhs.maxNm);
    const int N = (int)((endMax - beginMin)/(stepNmMin)) + 1;
    float* vals = new float[N];
    float nm = beginMin;
    for(int i = 0; i < N; i++){
        vals[i] = (*this)(nm) * (rhs)(nm);
        nm += stepNmMin;
    }
    Spectrum ret(vals, beginMin, endMax, stepNmMin, N);
    delete[] vals;
    Assert(N == ret.getNumSamples());
    return ret;
}


Spectrum Spectrum::operator/(const Spectrum& rhs)const{
    float stepNmMin = std::min<float>(stepNm, rhs.stepNm);
    float beginMin  = std::min<float>(minNm, rhs.minNm);
    float endMax    = std::max<float>(maxNm, rhs.maxNm);
    const int N = (int)((endMax - beginMin)/(stepNmMin)) + 1;
    float* vals = new float[N];
    float nm = beginMin;
    for(int i = 0; i < N; i++){
        vals[i] = (*this)(nm) / (rhs)(nm);
        nm += stepNmMin;
    }
    Spectrum ret(vals, beginMin, endMax, stepNmMin, N);
    delete[] vals;
    Assert(N == ret.getNumSamples());
    return ret;
}


Spectrum Spectrum::operator+(const Spectrum& rhs)const{
    float stepNmMin = std::min<float>(stepNm, rhs.stepNm);
    float beginMin  = std::min<float>(minNm, rhs.minNm);
    float endMax    = std::max<float>(maxNm, rhs.maxNm);
    const int N = (int)((endMax - beginMin)/(stepNmMin)) + 1;
    float* vals = new float[N];
    float nm = beginMin;
    for(int i = 0; i < N; i++){
        vals[i] = (*this)(nm) + (rhs)(nm);
        nm += stepNmMin;
    }
    Spectrum ret(vals, beginMin, endMax, stepNmMin, N);
    delete[] vals;
    Assert(N == ret.getNumSamples());
    return ret;
}


Spectrum Spectrum::operator-(const Spectrum& rhs)const{
    float stepNmMin = std::min<float>(stepNm, rhs.stepNm);
    float beginMin  = std::min<float>(minNm, rhs.minNm);
    float endMax    = std::max<float>(maxNm, rhs.maxNm);
    const int N = (int)((endMax - beginMin)/(stepNmMin)) + 1;
    float* vals = new float[N];
    float nm = beginMin;
    for(int i = 0; i < N; i++){
        vals[i] = (*this)(nm) - (rhs)(nm);
        nm += stepNmMin;
    }
    Spectrum ret(vals, beginMin, endMax, stepNmMin, N);
    delete[] vals;
    Assert(N == ret.getNumSamples());
    return ret;
}

float Spectrum::integrate()const{
    return defIntegralTrapezoid(getMinNm(), getMaxNm());
}

float Spectrum::defIntegralTrapezoid(float nmBegin, float nmEnd)const{

    Assert(nmBegin <= nmEnd);

    //Delta case
    if(nmBegin == nmEnd){
        return (*this)(nmBegin);
    }

    std::vector<float> sampNms = getSampleNms();

    //Find start and end indices for numerical integration
    int startIndex = -1;
    int endIndex   = -1;
    if(nmBegin <= sampNms[0]){
        startIndex = 0;
    }else{
        for(size_t i = 0; i < sampNms.size(); i++){
            if(sampNms[i] >= nmBegin){
                startIndex = (int)i;
                break;
            }
        }
    }
    if(nmEnd >= sampNms[sampNms.size()-1]){
        endIndex = sampNms.size()-1;
    }else{
        for(size_t i = 0; i < sampNms.size(); i++){
            if(sampNms[i] >= nmEnd){
                endIndex = (int)(i-1);
                break;
            }
        }
    }
    if(endIndex < startIndex){
        endIndex = startIndex;
    }

    //If the indices were not found, the range to integrated did not overlap
    //with the data
    if(startIndex == -1 || endIndex == -1){
        return 0.0f;
    }

    //Make sure indices are valid
    Assert(endIndex >= startIndex && endIndex != -1 && startIndex != -1);

    //Integrate the main region using
    //"Uniform Spacing Trapezoid Rule" given on page 191 of
    //"Numerical Mathematics and Computing" by Ward and Cheny - 6th ed
    float mainAccum = 0.0f;
    for(int i = startIndex + 1; i <= endIndex - 1; i++){
        mainAccum += (*this)[i];
    }
    const float mainContrib = (stepNm * (0.5f *
        ((*this)[startIndex] + (*this)[endIndex]) +
        mainAccum)) * (startIndex == endIndex ? 0.0f : 1.0f);
        //Contribution from main component of integral
        //(ignoring edge cases)
        //note that the contribution is 0 when start and end indices are the same

    //Now we must include edge cases: trapezoids that weren't fully in the integration
    //region
    float edgeTrapRight = 0.0f;
    float edgeTrapLeft  = 0.0f;

    if(nmBegin > sampNms[0] && nmBegin != sampNms[startIndex]){
        const float leftWidth = sampNms[startIndex] - nmBegin;
        const float leftA = (*this)(nmBegin);
        const float leftB = (*this)[startIndex];
        edgeTrapLeft = leftWidth * (0.5f * (leftA + leftB));
    }
    if(nmEnd < sampNms[sampNms.size()-1] && nmEnd != sampNms[endIndex]){
        const float rightWidth = nmEnd - sampNms[endIndex];
        const float rightA = (*this)[endIndex];
        const float rightB = (*this)(nmEnd);
        edgeTrapRight = rightWidth * (0.5f * (rightA + rightB));
    }

    //Result = main integral + both edge cases
    return mainContrib + edgeTrapLeft + edgeTrapRight;
}


std::ostream& operator<<(std::ostream& os, const Spectrum& s){
    os << "Spectrum: " << std::endl;
    os << "\tN = " << s.N << std::endl;
    os << "\tminNm = " << s.minNm << std::endl;
    os << "\tmaxNm = " << s.maxNm << std::endl;
    os << "\tstepNm = " << s.stepNm << std::endl;
    std::vector<float> nms = s.getSampleNms();
    for(int i = 0; i < s.N; i++){
        os << "\t\ts[" << nms[i] << "] = " <<
            //s(nms[i]) << std::endl;
            s[i] << std::endl;
    }
    return os;
}




