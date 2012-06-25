#include "parallel/RenderTask.h"
//--
#include "utils/Assert.h"
#include "color/ColorConstants.h"

RenderTask::RenderTask(ImageSampler::Sample* samples,
    const ImageSampler* imSampler, int Nsamps,
    const Camera* camera, ImageSensor* sensor,
    const Scene* sceneData, const Integrator* integrator) :
    samps(samples), sampler(imSampler), N(Nsamps), cam(camera), ccd(sensor),
    scene(sceneData), integ(integrator)
{
    Assert(samps   != NULL);
    Assert(sampler != NULL);
    Assert(N > 0          );
    Assert(cam     != NULL);
    Assert(ccd     != NULL);
    Assert(scene   != NULL);
    Assert(integ   != NULL);
}


RenderTask::~RenderTask(){}

//Tell the progress meter prog what we have finished "amt" of the total
//samples "tot."  If prog == NULL this does nothing.
static inline void updateProgress(ProgressMeter* prog, int amt){
    if(prog != NULL){
        prog->setProgress(amt);
    }
}


void RenderTask::render(ProgressMeter* prog){

    //We only update the progress meter(or initialize it at all)
    //if prog is non-NULL
    const bool areReportingProgress = prog != NULL;
    if(areReportingProgress){
        prog->startMeter(N);
    }

    //Loop through and render each sample
    const float invSamplerWidth  = 1.0f / sampler->getImageWidth();
    const float invSamplerHeight = 1.0f / sampler->getImageHeight();
    Spectrum black(0.0f, ColorConstants::MIN_NM_VIS, ColorConstants::MAX_NM_VIS);
    for(int i = 0; i < N; i++){

        //Compute image plane position and cast a primary ray
        const ImageSampler::Sample s = samps[i];
        const float xp = s.x * invSamplerWidth;
        const float yp = s.y * invSamplerHeight;
        const Ray ray = cam->generateRay(xp, yp);

        //Intersect primary ray with geometry
        bool didHit = false;
        RaySurfIntersection hit =
            scene->getSceneGeom()->getClosestIntersection(ray, didHit);

        //If we did hit geometry, use an integrator to compute radiance
        //otherwise, just add a black spectral sample
        Integrator::IntegratorData idata;
        if(didHit){
            Spectrum L = integ->radiance(scene, ray, hit, idata);
            ccd->addSample(s, &L);
        }else{
            ccd->addSample(s, &black);
        }
    }

    //Done!
    if(areReportingProgress){
        prog->finished();
    }
}
