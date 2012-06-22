#ifndef PATH_TRACING_INTEGRATOR_H
#define PATH_TRACING_INTEGRATOR_H

#include "integrators/Integrator.h"
#include "color/Spectrum.h"
#include "math/Vector.h"
#include "geom/Ray.h"

class PathTracingIntegrator : public Integrator{
public:
    PathTracingIntegrator(int maxNumBounces = 5, float probEmit = 0.25f);
    virtual Spectrum radiance(const Scene* scene, const Ray& ray, const RaySurfIntersection& hit)const;
private:
    Spectrum radianceHelper(const Scene* scene, const Ray& ray, const RaySurfIntersection& hit, int bounce)const;
    int maxBounces;
    float pEmit;
};

#endif //PATH_TRACING_INTEGRATOR_H
