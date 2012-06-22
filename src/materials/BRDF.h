#ifndef BRDF_H
#define BRDF_H

#include "math/Vector.h"
#include "utils/Assert.h"

/**
 *  Base class that all other BRDF models are derived from.
 */
class BRDF{
public:

    /**
     *  Evaluate the BRDF.  Vectors MUST be normalized!
     *
     *  @param w_i is the incident direction (incoming light).
     *  @param w_o is the outgoing direction (exitant direction).
     *  @param lambda is the wavelength.
     *  @return the BRDF value.
     */
    virtual float f(const Vector& w_i, const Vector& w_o, float lambda)const = 0;

    /**
     *  Evaluate the BRDF at many wavelengths at once.
     */
    void fAtMultipleWavelengths(
        const Vector& w_i, const Vector& w_o, //Direction vectors
        float* wavelengths, int Ns,           //Sample wavelengths and num-samps
        float* ret                            //Output array of size Ns
    )const;
};


//Inline function definitions -------------------------------------------------

inline void BRDF::fAtMultipleWavelengths(
    const Vector& w_i, const Vector& w_o, //Direction vectors
    float* wavelengths, int Ns,           //Sample wavelengths and num-samps
    float* ret)const                      //Output array of size Ns
{
    Assert(wavelengths != NULL && ret != NULL && Ns > 0);
    for(int i = 0; i < Ns; i++){
        ret[i] = f(w_i, w_o, wavelengths[i]);
    }
}


#endif //BRDF_H
