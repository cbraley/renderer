#include "IntegratorHelpers.h"
//--
#include "Assert.h"
#include "GeomUtils.h"
#include "Vector.h"
#include "BRDFGeom.h"



IntegratorHelpers::SpectralStrategy::SpectralStrategy() :
    //numSpectralSamples(33), nmMin(400.0f), nmMax(720.0f), nmStep(10.0f)
    numSpectralSamples(9), nmMin(400.0f), nmMax(800.0f), nmStep(50.0f)
{}

IntegratorHelpers::SpectralStrategy::SpectralStrategy(int nss, float nmmin,
    float nmmax, float nmstep) :
    numSpectralSamples(nss), nmMin(nmmin), nmMax(nmmax), nmStep(nmstep)
{}

Spectrum IntegratorHelpers::radianceDirect(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit, const IntegratorHelpers::SpectralStrategy& ss)
{
    //Memory for output spectrum
    float* buf = new float[ss.numSpectralSamples];
    for(int i = 0; i < ss.numSpectralSamples; i++){
        buf[i] = 0.0f;
    }

    //Return spectrum
    Spectrum retColor(buf, ss.nmMin, ss.nmMax, ss.nmStep, ss.numSpectralSamples);

    //Create relavent params
    const Vector omega_o(-ray.getDir());
    const Vector vecN(hit.n.x, hit.n.y, hit.n.z);
    const AccelStructure* geom = scene->getSceneGeom(); //Scene acceleration structure
    const bool isEmitter = hit.shp->isEmitter();
    const Point hitLocWS(hit.locWS);

    //Determine the maximum number of light samples we will make
    //and allocate this memory OUTSIDE of the light loop
    int NLightSampsMax = -1;
    for(size_t i = 0; i < scene->getNumLights(); i++){
        NLightSampsMax = std::max<int>(
            NLightSampsMax,
            scene->getLight(i)->getRecNumVisibilitySamples());
    }
    Point* samps = new Point[NLightSampsMax];

    //Process each light individually
    for(size_t i = 0; i < scene->getNumLights(); i++){
        Light* l = scene->getLight(i);

        //Skip sampling the light itself if the geometry is an emitter
        if(isEmitter && hit.shp->getEmitter() == l){
            continue;
        }

        //Sample the light source potentially many times
        const int N = l->getRecNumVisibilitySamples();
        l->getSamples(samps, N);


        //Loop over all light samples
        const float invNumSamps = 1.0f / float(N);
        for(int j = 0; j < N; j++){

            //Check if we can see the light
            VisibilityTester visTest(hitLocWS, samps[j], l->getGeom(),
                geom, DFLT_RAY_MOVE_EPSILON);
            //visTest.r is a ray to the light from hitLocWS

            //Get the incident radiance from the light at the hit location
            Spectrum L = l->sampleIncidentRadiance(hitLocWS, vecN, samps[j]);
            //TODO: "continue" here if the spectrum is black...is this any faster?


            //If we saw the light, add its contribution
            bool sawLight = visTest.visible(NULL);
            if(sawLight){
                //Vector to light
                const Vector omega_i(visTest.r.getDir());

                //Geomety term (solid angle)
                const Vector Nvec = hit.n;
                const float G = GeomUtils::absDotProd(omega_i, Nvec);
                //Technically, the above G is not correct! TODO
                //const Vector v = (samps[j] - hitLocWS);
                //const float r = v.magnitude();
                //const float G = GeomUtils::absDotProd(omega_i, Nvec) / (r*r);

                //Transform vectors into local BRDF frame
                /*
                Vector w_i_brdfCS = BRDFCoordHelpers::worldCSToBrdfCS(omega_i,
                   Nvec, hit.tangent, Nvec.cross(hit.tangent));
                Vector w_o_brdfCS = BRDFCoordHelpers::worldCSToBrdfCS(omega_o,
                   Nvec, hit.tangent, Nvec.cross(hit.tangent));
                */

                //Evaluate BRDF term at each wavelength
                float nm = ss.nmMin;
                BRDF* brdf = hit.shp->getBRDF();
                for(int i = 0; i < ss.numSpectralSamples; i++){
                    buf[i] = brdf->f(omega_i, omega_o, nm);
                    //buf[i] = brdf->f(w_i_brdfCS, w_o_brdfCS, nm);
                    nm += ss.nmStep;
                }

                //Create a "spectrum" of BRDF values
                Spectrum brdfSpec(buf, ss.nmMin, ss.nmMax, ss.nmStep,
                    ss.numSpectralSamples);

                //Light term
                retColor = retColor + (L * G * invNumSamps * brdfSpec);
            }
        }
    }

    //All done!
    delete[] samps; samps = NULL;
    delete[] buf; buf = NULL;
    return retColor;
}


Spectrum IntegratorHelpers::radianceEmitted(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit, const IntegratorHelpers::SpectralStrategy& ss)
{
    Point hitLocWS(hit.locWS);

    //Account for intersection directly with emissive
    //geometry(area lights)
    const bool isEmitter = hit.shp->isEmitter();
    Light* emitSrc = NULL;
    if(isEmitter){
        emitSrc = hit.shp->getEmitter();

        //When both points passed to sampleIncidentRadiance are the same,
        //the function knows we are sampling a light directly
        Spectrum Le = emitSrc->sampleIncidentRadiance(hitLocWS,
            Vector(1.0f,0.0f,0.0f), hitLocWS);
        return Le;
    }

    //If we are not on an emitter, return black
    return Spectrum(0.0f, ss.nmMin, ss.nmMax);
}



Spectrum IntegratorHelpers::radianceIndirect(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit, const IntegratorHelpers::SpectralStrategy& ss,
    int maxBounce, int bounce)
{
    const int NUM_SAMPS = 30; //How many times to sample the hemisphere?

    Spectrum ret(0.0f, ss.nmMin, ss.nmMax);//Black
    if(bounce > maxBounce){
        return ret;
    }

    const Vector normalVec(hit.n.x, hit.n.y, hit.n.z);
    const Vector omega_o(-ray.getDir());

    float* buf = new float[ss.numSpectralSamples];

    const float sampWeight = 1.0f / float(NUM_SAMPS);
    for(int i = 0; i < NUM_SAMPS; i++){

        //Choose random dir on unit hemisphere about normal
        Vector randomHemiVec = GeomUtils::randomVecOnSphere();
        if(randomHemiVec.dot(normalVec) < 0.0f){ //rejection sampling
            randomHemiVec = -randomHemiVec;
        }
        const Vector omega_i(randomHemiVec);

        //Trace a ray
        Ray reflRay(hit.locWS, randomHemiVec, true);
        bool newDidHit = false;
        RaySurfIntersection newHit =
            scene->getSceneGeom()->getClosestIntersection(reflRay, newDidHit);
        if(newDidHit){

            //Evaluate BRDF term at each wavelength
            float nm = ss.nmMin;
            BRDF* brdf = hit.shp->getBRDF();
            for(int i = 0; i < ss.numSpectralSamples; i++){
                buf[i] = brdf->f(omega_i, omega_o, nm);
                nm += ss.nmStep;
            }
            //Create a "spectrum" of BRDF values
            Spectrum brdfSpec(buf, ss.nmMin, ss.nmMax, ss.nmStep,
                ss.numSpectralSamples);


            return brdfSpec *
                    (radianceDirect(scene, reflRay, newHit, ss) +
                    radianceIndirect(scene, reflRay, newHit, ss, maxBounce, bounce + 1))
                    * sampWeight * PI_FLOAT;

            /*
            return
                brdfSpec *
                (
                    (radianceDirect(scene, reflRay, newHit, ss) +
                    radianceEmitted(scene, reflRay, newHit, ss) ) + /
                    radianceIndirect(scene, reflRay, newHit, ss, maxBounce, bounce + 1)
                ) *
                PI_FLOAT * sampWeight;
                */
        }
    }

    delete[] buf; buf = NULL;
    return ret;
}


