#include "integrators/PathTracingIntegrator.h"
//--
#include "utils/Assert.h"
#include "utils/GeomUtils.h"
#include "integrators/IntegratorHelpers.h"

/*
static const float INDIRECT_MULT = 5.0f;

PathTracingIntegrator::PathTracingIntegrator(int maxNumBounces, float probEmit) :
    maxBounces(maxNumBounces), pEmit(probEmit)
{
    Assert(maxBounces > 0);
    Assert(pEmit > 0.0f);
}

Spectrum PathTracingIntegrator::radiance(const Scene* scene,
    const Ray& ray, const RaySurfIntersection& hit)const
{
    //return radianceHelper(scene, ray, hit, 0);
    IntegratorHelpers::SpectralStrategy  strat;
    return IntegratorHelpers::radianceDirect  (scene, ray, hit, strat) + 
           IntegratorHelpers::radianceEmitted (scene, ray, hit, strat) +
           (IntegratorHelpers::radianceIndirect(scene, ray, hit, strat, maxBounces) * INDIRECT_MULT);
}

*/

