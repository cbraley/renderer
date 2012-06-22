#ifndef MATERIAL_H
#define MATERIAL_H

#include "color/Spectrum.h"
#include "math/Vector.h"

class Material{
public:
    virtual Spectrum getApproxSpectralReflectance()const = 0;
    virtual Spectrum evaluate(const Vector& L, const Vector& N, const Vector& H,
        const float visAmt, const Spectrum& lightColor)const = 0;
};


#endif //MATERIAL_H
