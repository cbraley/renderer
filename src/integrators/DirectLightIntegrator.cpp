#include "integrators/DirectLightIntegrator.h"
//--
#include "utils/Assert.h"
#include "utils/GeomUtils.h"
#include "lights/VisibilityTester.h"
#include "integrators/IntegratorHelpers.h"

DirectLightIntegrator::DirectLightIntegrator() {}

Spectrum DirectLightIntegrator::radiance(
    const Scene* scene,const Ray& ray,
    const RaySurfIntersection& hit, 
    Integrator::IntegratorData& iData )const
{
    IntegratorHelpers::SpectralStrategy  strat;
    return 
        IntegratorHelpers::radianceEmitted(scene, ray, hit, strat) + //Emitted light + 
        IntegratorHelpers::radianceDirect (scene, ray, hit, strat);  //Direct light

}

