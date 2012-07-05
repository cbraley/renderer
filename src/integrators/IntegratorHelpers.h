#ifndef INTEGRATOR_HELPERS_H
#define INTEGRATOR_HELPERS_H

#include <vector>
//--
#include "scene/Scene.h"
#include "geom/Ray.h"
#include "geom/RaySurfIntersection.h"
#include "color/Spectrum.h"


/**
 *  Many different integrators need to call
 *  common subroutines.  These common
 *  subroutines are contained in this namespace.
 *
 *  All are reentrant.
 */
namespace IntegratorHelpers{



    /**
     *  \brief Strategies for sampling a light in the scene.
     *  Enumeration describing possible strategies for sampling(choosing)
     *  a single light in the scene.
     *
     *  In general, we construct a discrete PDF with N entries where there are N
     *  light sources in the scene.  Lights are then sampled proportional to this
     *  PDF.  Each enum value specifies a different way for creating this discrete PDF.
     *  Note that for some simple strategies, the PDF is not explicitly constructed.
     */
    enum LightSamplingStrategy{
        UNIFORM /*! Uniform sampling(treats all lights equally). */ ,
        POWER_HEURISTIC /*! Choose lights according to total power.  Favors bright lights; but ignores visibility.  */
    };

    /**A LightSampler must be constructed for the scene before
     * we can randomly sample lights.  All precomputation related to light
     * sampling is stored inside of a LightSampler object.  Without this class,
     * we would have to re-compute data(CDFs, PDFs, etc.) each time we wished to
     * draw a sample.  This class' single member function "sampleLight" is responsible
     * for choosing a light.
     */
    class LightSampler{
    public:
        LightSampler(const Scene* scene, LightSamplingStrategy strat);

        /**
        *  Randomly sample(choose) a light in the scene according to a
        *  heuristic.  Ideally, this heuristic should allow us to more frequently select
        *  lights that contribute more to the final image(importance sampling).
        *
        *  The sample strategy is choen when the "sampler" was created.
        *
        *  @param uniformRand is a random floating point number in the range [0,1).
        *   Values MUST be inside this interval!
        *  @return a pointer to the light selected.  Returns NULL in the event that the
        *   scene had no lights.
        */
        Light* sampleLight(float uniformRand);


    private: // (Note: Non-Copyable)
        LightSampler();
        LightSampler& operator=(const LightSampler& rhs);
        const Scene* renScene;          //scene we are sampling lights from
        size_t numLights;               //Number of lights to choose from
        LightSamplingStrategy strategy; //What strat did we choose?
        std::vector<float> pdf;         //pdf of light power
        std::vector<float> cdf;         //cdf of light power

        void precomputePDFandCDF();     //helper function
        int sampleViaCDFInversionHelper(float uniformRand);
    };



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
     *  Return the amount of light emitted by the scene geometry at
     *  intersection point "hit."
     */
    Spectrum radianceEmitted(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, const SpectralStrategy& ss);


    /*
    Spectrum radianceIndirect(const Scene* scene, const Ray& ray,
        const RaySurfIntersection& hit, const SpectralStrategy& ss,
        int maxBounce, int bounce = 0);
    */

}

#endif //INTEGRATOR_HELPERS_H
