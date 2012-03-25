#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "Scene.h"
#include "Ray.h"
#include "RaySurfIntersection.h"
#include "Spectrum.h"

class Integrator{
public:

    /**
     *  Return the incident radiance at "hit" that is scattered back TOWARD the origin of ray "ray" along
     *  "ray.getDir()."
     */
    virtual Spectrum radiance(const Scene* scene, const Ray& ray, const RaySurfIntersection& hit)const = 0;
};

#endif //INTEGRATOR_H
