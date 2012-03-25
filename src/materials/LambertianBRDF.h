#ifndef LAMBERTIAN_BRDF_H
#define LAMBERTIAN_BRDF_H

#include "Spectrum.h"
#include "BRDF.h"
#include "Vector.h"

class LambertianBRDF : public BRDF{
public:
    LambertianBRDF();
    LambertianBRDF(const Spectrum& spectralRefl);
    virtual float f(const Vector& w_i, const Vector& w_o, float lambda)const;

    virtual ~LambertianBRDF();

    LambertianBRDF(const LambertianBRDF& rhs);
    LambertianBRDF& operator=(const LambertianBRDF& rhs);

private:
    Spectrum* specRef;
};

#endif //LAMBERTIAN_BRDF_H
