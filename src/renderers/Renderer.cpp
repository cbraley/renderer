#include "renderers/Renderer.h"
//--
#include <iostream>
//--
#include "utils/Assert.h"
#include "utils/Timer.h"

//#define SILENT
static const int PRINT_EVERY_X_PERCENT = 10;

void Renderer::preprocess(const Scene* scene, const Integrator* integrator)const{}

HDRImage* Renderer::render(ImageSampler* sampler, Camera* cam, ImageSensor* ccd,
    const Scene* scene, const Integrator* integrator)const
{
    Assert(sampler    != NULL);
    Assert(cam        != NULL);
    Assert(ccd        != NULL);
    Assert(scene      != NULL);
    Assert(integrator != NULL);

    const bool MAKE_RGB = true; //Set to false to get XYZ color
    const float MIN_NM_VIS = 400.0f;
    const float MAX_NM_VIS = 720.0f;

    //Preprocess and generate any temporary data structures needed by the renderer
    //preprocess(scene, integrator);
    //TODO: Call this from main!

    //Generate all of our samples
    const int NS = sampler->getNumSamplesForWholeImage();
    Assert(NS > 0);
    ImageSampler::Sample* samps = sampler->genSamples();
    Assert(samps != NULL);


#ifndef SILENT
    const int printEvery = (NS * PRINT_EVERY_X_PERCENT)/100; //At which sample indices should we print?
    Timer timer;
    timer.start();
#endif


    //Get radiance estimate for each sample
    //TODO: Parallelize by using multiple subwindows with 1 window per thread
    for(int i = 0; i < NS; i++){

        //Print something every X percent to show progress(only when SILENT is not defined)
#ifndef SILENT
        //TODO: This will cause race conditions upon parallelization; use atomics
        const float perc = (float(i) / float(NS)) * 100.0f;
        if(i % printEvery == 0 && (int)perc != 0){
            timer.stop();
            long double timeSoFar = timer.getElapsedSeconds() / 60.0;
            timer.start();
#TODO: Set flags for clang and visual studia            std::cout << "\tPercent complete: " << int(perc) <<
                "(image plane sample " << i << " of " << NS << ")" << std::endl;
            std::cout << "\t\tTime so far: " << timeSoFar << " minutes." << std::endl;
            long double estLeft = (timeSoFar * 100.0f) / perc;
            std::cout << "\t\tEstimated time remaining: " << estLeft << " minutes." << std::endl;
        }
#endif


        //Generate ray through the sample
        ImageSampler::Sample s = samps[i];
        //   Coords to generateRay should be in [0,1] range
        const float xp = s.x / float(sampler->getImageWidth());
        const float yp = s.y / float(sampler->getImageHeight());
        Ray ray = cam->generateRay(xp, yp);

        //Intersect ray through image plane with the scene
        bool didHit = false;
        RaySurfIntersection hit =
            scene->getSceneGeom()->getClosestIntersection(ray, didHit);

        //If we did hit geometry, use an integrator to compute radiance
        //otherwise, just return a black spectrum
        Spectrum pixelSampleColor;
        Integrator::IntegratorData idata;
        if(didHit){
            pixelSampleColor = integrator->radiance(scene, ray, hit, idata);
        }else{
            pixelSampleColor = Spectrum(0.0f, MIN_NM_VIS, MAX_NM_VIS);
        }

        //Add this sample to the image
        ccd->addSample(s, &pixelSampleColor);
    }

#ifndef SILENT
    timer.stop();
#endif

    //Cleanup mem
    delete[] samps; samps = NULL;

    //ImageSensor class takes care of interpolating samples
    //to get pixel values
    return ccd->toImage(MAKE_RGB);
}

