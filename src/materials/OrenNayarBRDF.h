#ifndef OREN_NAYAR_BRDF_H
#define OREN_NAYAR_BRDF_H

#include "materials/BRDF.h"
#include "color/Spectrum.h"
#include "math/Vector.h"

class OrenNayarBRDF : public BRDF{
    OrenNayarBRDF();
    OrenNayarBRDF(const Spectrum& sRefl, float sigmaVal);
    virtual float f(const Vector& w_i, const Vector& w_o, float lambda)const;
private:
    float sigma; //artist param
    float A,B;      //precomputed terms
    Spectrum refl;  //Wavelength dependence

    void precomputeAB();
};

#endif //OREN_NAYAR_BRDF_H
