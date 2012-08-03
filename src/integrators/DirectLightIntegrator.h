#ifndef DIRECT_LIGHT_INTEGRATOR_H
#define DIRECT_LIGHT_INTEGRATOR_H

#include "integrators/Integrator.h"

/**
 *  Integrator that only accounts for direct light that passes from the light source 
 *  directly to the material surface.  No bounces are accounted for.
 */
class DirectLightIntegrator : public Integrator{
public:

    DirectLightIntegrator();

    virtual Spectrum radiance(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, 
        Integrator::IntegratorData& iData
        )const;

};

#endif //DIRECT_LIGHT_INTEGRATOR_H
