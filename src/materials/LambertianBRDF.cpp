#include "materials/LambertianBRDF.h"
//--
#include "utils/Assert.h"
#include "utils/Constants.h"

//#define ASSERT_FAIL_WHEN_OUTSIDE_WAVELEN_RANGE

LambertianBRDF::LambertianBRDF(){
    specRef = new Spectrum(1.0f, 400.0f, 720.0f);
}

LambertianBRDF::LambertianBRDF(const Spectrum& spectralRefl){
    specRef = new Spectrum(spectralRefl);
}

float LambertianBRDF::f(const Vector& w_i, const Vector& w_o,
    float lambda)const
{
    //Assert precondition of normalization
    Assert(w_i.isNormalized());
    Assert(w_o.isNormalized());

    //Verify wavelength range
#ifdef ASSERT_FAIL_WHEN_OUTSIDE_WAVELEN_RANGE
    Assert(lambda >= specRef->getMinNm() &&
        lambda <= specRef->getMaxNm());
#endif

    //Sample the BRDF
    return (*specRef)(lambda) * INVERSE_PI_FLOAT;
}


LambertianBRDF::LambertianBRDF(const LambertianBRDF& rhs){
    specRef = new Spectrum(*(rhs.specRef));
}

LambertianBRDF& LambertianBRDF::operator=(const LambertianBRDF& rhs){
    specRef = new Spectrum(*(rhs.specRef));
    return *this;
}

LambertianBRDF::~LambertianBRDF(){
    delete specRef; specRef = NULL;
}

