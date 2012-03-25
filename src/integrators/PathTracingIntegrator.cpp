#include "PathTracingIntegrator.h"
//--
#include "Assert.h"
#include "RNGs.h"
#include "GeomUtils.h"
#include "IntegratorHelpers.h"

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


/*
Spectrum PathTracingIntegrator::radianceHelper(const Scene* scene,
    const Ray& ray, const RaySurfIntersection& hit, int bounce)const
{
    Assert(hit.didHit);

    //Constants
    const int N_S       = 10;     //Num spectral samples
    const float NM_MIN  = 400.0f; //Wavelength start
    const float NM_MAX  = 720.0f;
    const float NM_STEP = 10.0f;
    const int N_RADIANCE_SAMPLES = 10;

    //Memory for output spectrum
    float* buf = new float[N_S];
    for(int i = 0; i < N_S; i++){
        buf[i] = 0.0f;
    }

    //Return spectrum
    Spectrum black(buf, NM_MIN, NM_MAX, NM_STEP, N_S);
    Spectrum retColor(black);

    //Maybe quit early
    if(bounce >= maxBounces){
        delete[] buf; buf = NULL;
        return retColor;
    }

    //Create relavent params
    const Vector omega_o(-ray.getDir());
    const Normal& n = hit.n;
    const Vector vecN(n.x, n.y, n.z);
    const AccelStructure* geom = scene->getSceneGeom(); //Scene acceleration structure

    //Compute a point hitLocWS that is the hit location in world space, moved out SLIGHTLY
    //away from the object to avoid self intersection
    Point hitLocWS(hit.locWS);
    Ray dummy(hitLocWS, vecN, true);
    dummy.moveOriginSlight();
    hitLocWS = dummy.getOrigin();

    //Random var
    const float pRand = RNG::randomFloat(0.0f, 1.0f);
    const bool randEmit = pRand > pEmit;
    const float emitMult = 1.0f / randEmit;
    const float refMult  = 1.0f / (1.0f - randEmit);

    //Account for intersection directly with emissive
    //geometry(area lights)
    const bool isEmitter = hit.shp->isEmitter();
    if(randEmit){
        Light* emitSrc = NULL;
        if(isEmitter){
            emitSrc = hit.shp->getEmitter();

            //We don't sample radiance at a point in space since we have DIRECTLY hit the light
            //Spectrum Le = emitSrc->getSPD();
            Spectrum Le = emitSrc->sampleIncidentRadiance(hitLocWS,
                Vector(1.0f,0.0f,0.0f), hitLocWS);
            delete[] buf; buf = NULL;
            return retColor + (Le * emitMult);
        }
        return retColor;
    }

    //Hit the light but weren't randomly looking for an emitter
    if( (isEmitter && (!randEmit)) ){
        delete[] buf; buf = NULL;
        return retColor;
    }

    //Generate some random hemispeherical samples
    for(int j = 0; j < N_RADIANCE_SAMPLES; j++){
        Vector omega_i = GeomUtils::randomVecOnSphere(); //TODO: should be hemi
        const Vector Nvec(n.x, n.y, n.z);
        const float G = GeomUtils::absDotProd(omega_i, Nvec);

        //Evaluate BRDF term at each wavelength
        float nm = NM_MIN;
        BRDF* brdf = hit.shp->getBRDF();
        for(int i = 0; i < N_S; i++){
            buf[i] = brdf->f(omega_i, omega_o, nm);
            nm += NM_STEP;
        }

        //Create a "spectrum" of BRDF values
        Spectrum brdfSpec(buf, NM_MIN, NM_MAX, NM_STEP, N_S);


        //Shoot a recursive ray
        Ray newRay(hitLocWS, omega_i, true);
        newRay.moveOriginSlight();
        bool needToRecur = false;
        RaySurfIntersection newHit = geom->getClosestIntersection(newRay, needToRecur);
        Spectrum recurSpec = black;
        if(needToRecur){
            recurSpec = radianceHelper(scene, newRay, newHit, bounce + 1);
        }
        retColor = retColor + (brdfSpec * recurSpec) * (refMult * G);
    }
    delete[] buf; buf = NULL;
    return retColor;

}
*/
