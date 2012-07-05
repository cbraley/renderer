#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene/Scene.h"
#include "geom/Ray.h"
#include "geom/RaySurfIntersection.h"
#include "color/Spectrum.h"

/**
 *  Abstract(pure-virtual) base class for all other integrators.  Contains a 
 *  single function that must be implemented "radiance."  This function returns the 
 *  outgoing(exitant) radiance at the ray-surface intersection described by "hit" 
 *  along the geometric ray "ray."
 *
 *
 *  Integrators, in general, approximate the equation:
 *  \f$  L_o(p, \hat{\omega_o}) = L_e(p, \hat{\omega_o})  + \int_{H}   f(p,\hat{\omega_o},\hat{\omega_i}) L_o(p,\hat{\omega_i})  |\cos(\theta_i)|  d \omega_i  \f$
 *
 */
class Integrator{
public:

    /**
     *  Misc data required by integrators.
     */
    typedef struct IntegratorData{
        //TODO: Memory and samples should eventually go here   
    }IntegratorData;

    /**
    *  This functions function returns the 
    *  outgoing(exitant) radiance at the ray-surface intersection described by "hit" 
    *  along the geometric ray "ray."
    */
    virtual Spectrum radiance(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, IntegratorData& iData
        )const = 0;
};

#endif //INTEGRATOR_H
