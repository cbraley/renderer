#include "OrenNayarBRDF.h"
//--
#include "Assert.h"
#include "Constants.h"

void OrenNayarBRDF::precomputeAB(){
    const float sigmaSquared = sigma * sigma;
    A = 1.0f - (sigmaSquared /
        (2.0f * (sigmaSquared + 0.33f)) );
    B = (0.45f * sigmaSquared) / (sigmaSquared + 0.09f);
}

OrenNayarBRDF::OrenNayarBRDF() :
    sigma(1.0f), refl(1.0f, 400.0f, 720.0f)
{
    precomputeAB();
}

OrenNayarBRDF::OrenNayarBRDF(const Spectrum& sRefl, float sigmaVal) :
    sigma(sigmaVal), refl(sRefl)
{
    precomputeAB();
}


float OrenNayarBRDF::f(const Vector& w_i, const Vector& w_o, float lambda)const{
    //TODO: Compute this - See page 449 of PBRT
    Assert(false);
    return 0.0f;
}

