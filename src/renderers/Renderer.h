#ifndef RENDERER_H
#define RENDERER_H

#include "Ray.h"
#include "Scene.h"
#include "ImageSensor.h"
#include "ImageSampler.h"
#include "Camera.h"
#include "RaySurfIntersection.h"
#include "Integrator.h"
#include "ImageTypes.h"


/**
 *  Base class for all renderers(witted raytracers, path tracers, etc).
 *  Renderers that don't shoot rays through the image plane(PRT renderers,
 *  radiosity, etc.) will likely need to have a new, more general, renderer
 *  base class written.
 */
class Renderer{
public:

    /**
     *  Do any kinf of preprocessing necessary.  The default implementation does nothing.
     *  This is always called before rendering begins.  Preprocessing might include
     *  creating any kind of precomputed tables or data needed.
     *  @param scene is the same Scene that will be used in calls to "radiance."
     */
    virtual void preprocess(const Scene* scene, const Integrator* integrator)const;

    /**
     *  Render the scene to produce a tristimulus image.  Default implementation works
     *  as expected, but this can be overridden as well.
     */
    virtual HDRImage* render(ImageSampler* sampler, Camera* cam, ImageSensor* ccd,
        const Scene* scene, const Integrator* integrator)const;
};


#endif //RENDERER_H
