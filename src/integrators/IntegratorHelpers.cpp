#include "integrators/IntegratorHelpers.h"
//--
#include "utils/Assert.h"
#include "utils/GeomUtils.h"
#include "math/Vector.h"
#include "materials/BRDFGeom.h"
//--
#include <algorithm>


//Implementation of light sampling code(LightSampler class) -------------------

IntegratorHelpers::LightSampler::LightSampler(const Scene* scene, LightSamplingStrategy strat) :
   renScene(scene), strategy(strat) 
{
    Assert(scene != NULL);
    
    numLights = scene->getNumLights();


    //Build PDF and CDF
    precomputePDFandCDF();
}


void IntegratorHelpers::LightSampler::precomputePDFandCDF(){
    Assert(pdf.empty() && cdf.empty());

    //Reserve space in PDF and CDF to avoid constantly growing the 
    //internal array
    pdf.reserve(renScene->getNumLights()    );
    cdf.reserve(renScene->getNumLights() + 1); //CDF is one larger since 0 is included

    //Find sum of total power...
    float sumTotPow = 0.0f;
    for(size_t i = 0; i < renScene->getNumLights(); i++){
        const float currPow = renScene->getLight(i)->totalPower();
        Assert(currPow >= 0.0f);
        sumTotPow += currPow;
    }
    Assert(sumTotPow >= 0.0f);

    //...use power sum to compute PDF and compute CDF
    //normalized to [0,1] as opposed to [0, totalPow]
    const float invTotalPow = 1.0f / sumTotPow; //* is faster than /
    float probSum = 0.0f;
    cdf.push_back(0.0f);
    for(size_t i = 0; i < renScene->getNumLights(); i++){
        const float currP = renScene->getLight(i)->totalPower() * invTotalPow;
        pdf.push_back(currP);
        probSum += currP;
        cdf.push_back(probSum);
    }
    
    //PDF and CDF are now tabulated; make sure that the resulting data
    //is reasonable
    Assert(MathUtils::checkClose<float>(probSum, 1.0f, 1e-6)); //CDF final value should be 1(with some FP error)
    Assert(pdf.size() + 1 == cdf.size()); //CDF is one larger since 0 is included

    //Make sure final CDF value is exactly 1 s.t no random numbers can be generated that are
    //larger than the largest CDF value.  Due to FP issues, it is possible that the final CDF 
    //value might be 1 - 1e-6.  Very rarely, our random number might be 1 - 1e-10.
    //In this case, our random number would exceed our largest CDF value and after 
    //sampleViaCDFInversionHelper is called the resulting index would cause an off-by-one
    //error when choosing a light...
    cdf[cdf.size()-1] = 1.0f;    
}


int IntegratorHelpers::LightSampler::sampleViaCDFInversionHelper(float uniformRand){
    Assert(uniformRand >= 0.0f && uniformRand < 1.0f);

    //Conduct a binary search along the CDF values to find bin closest to our value
    std::vector<float>::iterator resultIter;
    resultIter = std::lower_bound(cdf.begin(), cdf.end(), uniformRand);

    //We can subtract iterators to yield the array index
    //see the example here:  http://www.cplusplus.com/reference/algorithm/lower_bound/
    const int foundIndex = int(resultIter - cdf.begin());
    Assert(foundIndex >= 0 && foundIndex < ((int)renScene->getNumLights()));
    return foundIndex;
}

Light* IntegratorHelpers::LightSampler::sampleLight(float uniformRand){
    Assert(uniformRand >= 0.0f && uniformRand < 1.0f);

    int index = -1;
    switch(strategy){
        case UNIFORM: //Choose any light with equal prob. for each
            index = (int)(numLights * uniformRand);
            break;
        case POWER_HEURISTIC: //Importance sample according to power
            index = sampleViaCDFInversionHelper(uniformRand);
            break;
        default:
            //Error - We didn't account for an enum choice!
            Assert(false);
            return NULL;
    }
    
    Assert(index < numLights || numLights == 0);

    //Return NULL if there are no lights in the scene
    return numLights != 0 ? renScene->getLight(index) : NULL;
}

//-----------------------------------------------------------------------------



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



/*
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

        }
    }

    delete[] buf; buf = NULL;
    return ret;
}
*/


