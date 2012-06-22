#include "integrators/DirectLightIntegrator.h"
//--
#include "utils/Assert.h"
#include "utils/GeomUtils.h"
#include "lights/VisibilityTester.h"
#include "integrators/IntegratorHelpers.h"

DirectLightIntegrator::DirectLightIntegrator() {}

Spectrum DirectLightIntegrator::radiance(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit)const
{
    IntegratorHelpers::SpectralStrategy  strat;
    return IntegratorHelpers::radianceDirect(scene, ray, hit, strat) + //A - 3 vertex paths
        IntegratorHelpers::radianceEmitted(scene, ray, hit, strat);    //B - 2 vertex paths
}
