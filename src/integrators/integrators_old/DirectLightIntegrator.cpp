#include "DirectLightIntegrator.h"
//--
#include "Assert.h"
#include "GeomUtils.h"
#include "VisibilityTester.h"
#include "BRDFGeom.h"


DirectLightIntegrator::DirectLightIntegrator() {}



Spectrum DirectLightIntegrator::radiance(const Scene* scene, const Ray& ray,
    const RaySurfIntersection& hit)const
{
    //
    //  We are computing the following sum:
    //     retColor = sum_from_i_to_N (contribution from light i)
    //
    //  To compute contribution from light i we:
    //    consider directions:
    //      omega_o = - ray direction
    //      omega_i =  Vector to light
    //      N       =  Normal vector
    //    and BRDF:
    //      f(omega_i, omega_o, wavelength)
    //
    //    add emissive term if surface is emissive
    //
    //   (do for all wavelengths)
    //    genete M samples on light i:
    //    for each sample from j = 1 to M:
    //        compute omega_i
    //        if sample is visible:
    //            retColor += f(omega_i, omega_o) * G * Light in direction
    //            G is |cos(theta)|
    //

    //Constants
    const int N_S       = 33;     //Num spectral samples
    const float NM_MIN  = 400.0f; //Wavelength start
    const float NM_MAX  = 720.0f;
    const float NM_STEP = 10.0f;

    //Memory for output spectrum
    float* buf = new float[N_S];
    for(int i = 0; i < N_S; i++){
        buf[i] = 0.0f;
    }

    //Return spectrum
    Spectrum retColor(buf, NM_MIN, NM_MAX, NM_STEP, N_S);

    //Create relavent params
    const Vector omega_o(-ray.getDir());
    const Vector vecN = hit.n;
    const AccelStructure* geom = scene->getSceneGeom(); //Scene acceleration structure

    //Compute a point hitLocWS that is the hit location in world space, moved out SLIGHTLY
    //away from the object to avoid self intersection
    Point hitLocWS(hit.locWS);
    Ray dummy(hitLocWS, vecN, true);
    dummy.moveOriginSlight();
    hitLocWS = dummy.getOrigin();


    //Account for intersection directly with emissive
    //geometry(area lights)
    const bool isEmitter = hit.shp->isEmitter();
    Light* emitSrc = NULL;
    if(isEmitter){
        emitSrc = hit.shp->getEmitter();

        //We don't sample radiance at a point in space since we have DIRECTLY hit the light
        //Spectrum Le = emitSrc->getSPD();
        Spectrum Le = emitSrc->sampleIncidentRadiance(hitLocWS,
            Vector(1.0f,0.0f,0.0f), hitLocWS);
        retColor = retColor + Le;
        delete[] buf; buf = NULL;
        return retColor;
    }

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
        if(isEmitter && emitSrc == l){
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
            //TODO: "continue" here if the spectrum is black


            //If we saw the light, add its contribution
            bool sawLight = visTest.visible(NULL);
            if(sawLight){
                //Vector to light
                const Vector omega_i(visTest.r.getDir());

                //Geomety term (solid angle)
                const float G = GeomUtils::absDotProd(omega_i, vecN);
                //Assert(G >= 0.0f && G < 1.0f);

                //Evaluate BRDF term at each wavelength

                //Transform the omega vectors into the local CS
                //TODO: Do this!
                float nm = NM_MIN;
                BRDF* brdf = hit.shp->getBRDF();
                for(int i = 0; i < N_S; i++){
                    buf[i] = brdf->f(omega_i, omega_o, nm);
                    nm += NM_STEP;
                }

                //Create a "spectrum" of BRDF values
                Spectrum brdfSpec(buf, NM_MIN, NM_MAX, NM_STEP, N_S);

                //Light term
                retColor = retColor + (L * G * invNumSamps * brdfSpec);
            }


            /*
            //Turn on for debugging to see if we are properly sampling the light
            if(sawLight)
                retColor = Spectrum(Spectrum::CIE_ILLUM_E);
            else
                retColor = Spectrum(0.0f, 400.0f, 720.0f);
            */

        }
    }

    //All done!
    delete[] samps; samps = NULL;
    delete[] buf; buf = NULL;
    return retColor;
}
