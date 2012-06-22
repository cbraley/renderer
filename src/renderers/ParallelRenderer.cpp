#include "renderers/ParallelRenderer.h"
//--
#include <iostream>
//--
#include "utils/Assert.h"
#include "utils/Timer.h"
#include "color/ColorConstants.h"
//--
#include <pthread.h>


//Clamp to range [minInt, maxInt)
//Note that the upper bound is EXCLUSIVE
static inline int clampInteger(int v, int minInt, int maxInt){
    return v >= maxInt ? maxInt-1 : (v <= minInt ? minInt : v);
}

typedef struct ImRegion{
    int left, right, top, bottom;
    ImRegion(int l, int r, int t, int b) :
        left(l), right(r), top(t), bottom(b)
    { Assert(left < right && top < bottom); }

    /// Generate samples into "ret."  Return how many samples were created.
    inline int genSamples(ImageSampler* sampler, ImageSampler::Sample*& ret)const{
        sampler->getSamplesForSubwindow(left, right, top, bottom, ret);
        return sampler->getNumSamplesForWindow(left, right, top, bottom);
    }

    friend std::ostream& operator<<(std::ostream& os, const ImRegion& r){
        os << "( x(left,right) = [" << r.left << ", " << r.right << "],   y(top,bottom) = [" <<
            r.top << ", " << r.bottom << "])";
        return os;
    }

}ImRegion;


static std::list<ImRegion> generateTaskList(ImageSampler* sampler, int spwx, int spwy){
    std::list<ImRegion> ret;

    //Make sure that our window sizes are smaller than the image plane sizes
    const int winWidth   = spwx > sampler->getImageWidth()  ?
        sampler->getImageWidth()  : spwx;
    const int winHeight  = spwy > sampler->getImageHeight() ?
        sampler->getImageHeight() : spwy;
    Assert(winWidth > 0 && winHeight > 0);

    //Make all the samples
    const int nx = sampler->getImageWidth()  / winWidth;  //Num windows X
    const int ny = sampler->getImageHeight() / winHeight; //Num windows Y
    for(int ix = 0; ix < nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            //Construct a region, clamp to inside the image bounds
            //TODO: This may mess up reconstruction filters with a large spatial extent near image edges!
            const int left   = ix * winWidth;
            const int right  = left + winWidth;
            const int top    = iy * winHeight;
            const int bottom = top + winHeight;
            ImRegion region(left, right, top, bottom);
            region.left   = clampInteger(region.left  , 0, sampler->getImageWidth() );
            region.right  = clampInteger(region.right , 0, sampler->getImageWidth() );
            region.top    = clampInteger(region.top   , 0, sampler->getImageHeight());
            region.bottom = clampInteger(region.bottom, 0, sampler->getImageHeight());
            ret.push_back(region);
        }
    }
    return ret;
}


ParallelRenderer::ParallelRenderer(int sampsPerWinX, int sampsPerWinY, int numThreads) :
    spwx(sampsPerWinX), spwy(sampsPerWinY), nThreads(numThreads)
{
   Assert(nThreads     > 0);
   Assert(sampsPerWinX > 0);
   Assert(sampsPerWinY > 0);
}

void ParallelRenderer::preprocess(const Scene* scene, const Integrator* integrator)const{}


/**
 *  The void* argument "arg" passed to threadDoWork
 *  is of this type.
 */
typedef struct ThreadArgument{
    pthread_mutex_t* taskListMutexAddr; //Ptrto mutex for accessing task list
    std::list<ImRegion>* taskListPtr;   //Pointer to task list

    pthread_barrier_t* barrier;        //Each thread waits here when finishing

    ImageSampler::Sample* samples;     //Sample buffer for this thread.

    //Data to use to render with
    ImageSampler* sampler;
    ImageSensor* sensor;
    const Scene* scene;
    Camera* cam;
    const Integrator* integrator;

    //Data for identifing the thread
    int tid;
}ThreadArgument;

static void* threadDoWork(void* argument){
    Assert(argument != NULL);
    ThreadArgument arg = *((ThreadArgument*)argument);

    while(true){

        //Check for work to do
        pthread_mutex_lock(arg.taskListMutexAddr);
        //If there is no work, give back the lock and return
        const bool noWorkToDo = arg.taskListPtr->empty();
        if(noWorkToDo){
            pthread_mutex_unlock(arg.taskListMutexAddr);
            std::cout << "Thread: " << arg.tid << " has no more work to do..." << std::endl;
            pthread_barrier_wait(arg.barrier);
            return NULL;
        }
        //Get the work
        ImRegion region = arg.taskListPtr->front();
        arg.taskListPtr->pop_front();
        int workLeft = arg.taskListPtr->size();
        std::cout << "Thread: " << arg.tid << " took 1 unit of work, leaving: " << workLeft << " units..." << std::endl;
        std::cout << "\t" << region << std::endl;
        pthread_mutex_unlock(arg.taskListMutexAddr);

        //Generate samples for this region
        const int numSamples = region.genSamples(arg.sampler, arg.samples);

        //Do the rendering of each sample sequentially within this thread
        for(int i = 0; i < numSamples; i++){
            ImageSampler::Sample s = arg.samples[i];
            const float xp = s.x / float(arg.sampler->getImageWidth());
            const float yp = s.y / float(arg.sampler->getImageHeight());
            const Ray ray = arg.cam->generateRay(xp, yp);

            //Intersect ray through image plane with the scene
            bool didHit = false;
            RaySurfIntersection hit =
                arg.scene->getSceneGeom()->getClosestIntersection(ray, didHit);

            //If we did hit geometry, use an integrator to compute radiance
            //otherwise, just return a black spectrum
            Spectrum pixelSampleColor;
            if(didHit){
                pixelSampleColor = arg.integrator->radiance(arg.scene, ray, hit);
            }else{
                pixelSampleColor = Spectrum(0.0f,
                    ColorConstants::MIN_NM_VIS, ColorConstants::MAX_NM_VIS);
            }
            arg.sensor->addSample(s, &pixelSampleColor);
        }
    }

    //We will never reach this code...
    pthread_barrier_wait(arg.barrier);
    return NULL;
}

HDRImage* ParallelRenderer::render(ImageSampler* sampler, Camera* cam, ImageSensor* ccd,
    const Scene* scene, const Integrator* integrator)const
{


    //Figure out the MAXIMUM number of samples that could be within a subwindow
    const int maxNumSamps = sampler->getNumSamplesForWindow(0, spwx, 0, spwy);

    //Allocate 1 sample buffer per thread
    ImageSampler::Sample** sampleBufs = new ImageSampler::Sample*[nThreads];
    for(int i = 0; i < nThreads; i++){
        sampleBufs[i] = new ImageSampler::Sample[maxNumSamps];
    }

    //Allocate one tid and one arg per thread
    pthread_t* tids = new pthread_t[nThreads];
    ThreadArgument* args = new ThreadArgument[nThreads];

    //Get a list of windows to render
    std::list<ImRegion> taskList = generateTaskList(sampler, spwx, spwy);


    //We can't have multiple threads access the task list simeltaneously due
    //to race conditions....declare a mutex that must be acquired around all
    //operations on taskList
    pthread_mutex_t taskListLock;
    pthread_mutex_init(&taskListLock, NULL);


    //Initialize a barrier that all the threads wait at until everyone finishes
    pthread_barrier_t finishBarrier;
    pthread_barrier_init( &finishBarrier, NULL, nThreads);


    //Launch a series of threads
    for(int i = 0; i < nThreads; i++){

        //Setup this threads argument
        ThreadArgument arg;
        arg.taskListMutexAddr = &taskListLock;
        arg.taskListPtr       = &taskList;
        arg.barrier           = &finishBarrier;
        arg.samples           = sampleBufs[i];

        arg.sampler    = sampler;
        arg.sensor     = ccd;
        arg.scene      = scene;
        arg.cam        = cam;
        arg.integrator = integrator;

        arg.tid = i;

        args[i] = arg;

        //Launch the thread
        const int threadOK = pthread_create(
            &(tids[i]),   //Address of threads tid
            NULL,         //attribute(not needed)
            threadDoWork, //Function to start with
            (void*) &(args[i])
        );
        Assert(threadOK == 0);
    }


    //Wait on each thread
    for(int i = 0; i < nThreads; i++){
        int joinOK = pthread_join(tids[i], NULL);
        if(joinOK != 0){
            std::cerr << "ERROR - pthread with tid " << tids[i] <<
                " finished with error code: " << joinOK << std::endl;
        }
    }

    //TODO: Include some robust way to figure out when we are "done" rendering
    //Potentially include a condition variable indicating "done rendering" that each thread
    //checks every so often.  This variable can be controlled by a "boss" thread with lower priority which
    //waits for an action from the user in the GUI that indicates render completion, or waiting for the
    //total # of samples or time to exceed a certain amount.


    //Free all dynamiclly allocated memory
    for(int i = 0; i < nThreads; i++){
        delete[] sampleBufs[i]; sampleBufs[i] = NULL;
    }
    delete[] sampleBufs; sampleBufs = NULL;
    delete[] tids;       tids       = NULL;
    delete[] args;       args       = NULL;

    //Cleanup the mutex and barrier
    pthread_mutex_destroy(&taskListLock);
    pthread_barrier_destroy(&finishBarrier);

    //Return the final result
    return ccd->toImage(makeRgb);
}





