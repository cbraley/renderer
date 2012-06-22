#ifndef DIRECT_LIGHT_INTEGRATOR_H
#define DIRECT_LIGHT_INTEGRATOR_H

#include "integrators/Integrator.h"

class DirectLightIntegrator : public Integrator{
public:

    DirectLightIntegrator();

    virtual Spectrum radiance(const Scene* scene, const Ray& ray, const RaySurfIntersection& hit)const;
};

#endif //DIRECT_LIGHT_INTEGRATOR_H
