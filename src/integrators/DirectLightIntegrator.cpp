#include "DirectLightIntegrator.h"
//--
#include "Assert.h"
#include "GeomUtils.h"
#include "VisibilityTester.h"
#include "IntegratorHelpers.h"

DirectLightIntegrator::DirectLightIntegrator() {}

Spectrum DirectLightIntegrator::radiance(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit)const
{
    IntegratorHelpers::SpectralStrategy  strat;
    return IntegratorHelpers::radianceDirect(scene, ray, hit, strat) + //A - 3 vertex paths
        IntegratorHelpers::radianceEmitted(scene, ray, hit, strat);    //B - 2 vertex paths
}
