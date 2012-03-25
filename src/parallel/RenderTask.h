#ifndef RENDER_TASK_H
#define RENDER_TASK_H

#include "ImageSampler.h"
#include "ImageSensor.h"
#include "Camera.h"
#include "Integrator.h"
#include "Scene.h"
#include "ProgressMeter.h"

/**
 *  Base class that all other RenderTasks must derive from.
 *  Classes must implement the pure-virtual "render"
 *  function which renders an image and updaets the ProgressMeter
 *  object as it renders the series of samples.
 */
class RenderTask{
public:

    /**
     *  Create a renderer task.  Note that none of the pointers
     *  passed in are freed by the RenderTask upon completion!
     */
    RenderTask(ImageSampler::Sample* samples,
        const ImageSampler* imSampler, int Nsamps,
        const Camera* camera, ImageSensor* sensor,
        const Scene* sceneData, const Integrator* integrator);

    /**
     *  Destructor.
     */
    virtual ~RenderTask();

    /**
     *  Render the set of samples.  Progress will be updated
     *  to the progress meter "prog" if it is non-null.
     *
     *  Derived classes can override this function.  The default implementation
     *  processes each sample sequentially, and updates the progress counter
     *  with a predefined frequency.  This frequency is coded as a constant
     *  in RenderTask.cpp.
     *
     *  @param prog is the progress meter. Updated periodically,
     *  unless it is NULL.  If prog is NULL, it is ignored.  Defaults
     *  to NULL.
     */
    virtual void render(ProgressMeter* prog = NULL);

protected:
    ImageSampler::Sample* samps;
    const ImageSampler* sampler;
    int N;
    const Camera* cam;
    ImageSensor* ccd;
    const Scene* scene;
    const Integrator* integ;

private: //Helper functions
};


#endif //RENDER_TASK_H
