#ifndef INTEGRATOR_HELPERS_H
#define INTEGRATOR_HELPERS_H

#include "Scene.h"
#include "Ray.h"
#include "RaySurfIntersection.h"
#include "Spectrum.h"

/**
 *  Many different integrators need to call
 *  common subroutines.  These common
 *  subroutines are contained in this namespace.
 *
 *  All are reentrant.
 */
namespace IntegratorHelpers{

    typedef struct SpectralStrategy{
        SpectralStrategy();
        SpectralStrategy(int nss, float nmmin, float nmmax, float nmstep);
        int numSpectralSamples;
        float nmMin, nmMax, nmStep;
    }SpectralStrategy;

    /**
     *  Estimate ONLY the direct incident radiance in arriving in
     *  to point hit.locWS.  Does NOT include emitted light!
     */
    Spectrum radianceDirect(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, const SpectralStrategy& ss);


    /**
     *  Only sample DIRECT light.
     */
    Spectrum radianceEmitted(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, const SpectralStrategy& ss);


    Spectrum radianceIndirect(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, const SpectralStrategy& ss, 
        int maxBounce, int bounce = 0);

}

#endif //INTEGRATOR_HELPERS_H
