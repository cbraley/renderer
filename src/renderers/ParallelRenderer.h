#ifndef PARALLEL_RENDERER_H
#define PARALLEL_RENDERER_H

#include <list>
//--
#include "renderers/Renderer.h"

class ParallelRenderer : public Renderer{
public:

    //static void computeWindowSize(int imWidth, int imHeight, int threadCount, int goalNumRounds,
    //    int& sppX, int& sppY);

    ParallelRenderer(int sampsPerWinX = 128, int sampsPerWinY = 128, int numThreads = 1);

    virtual void preprocess(const Scene* scene, const Integrator* integrator)const;

    virtual HDRImage* render(ImageSampler* sampler, Camera* cam, ImageSensor* ccd,
        const Scene* scene, const Integrator* integrator)const;

    void setOutputRGB(bool rgb);
    void setOutputXYZ(bool xyz);
    bool isOutputRGB()const;
    bool isOutputXYZ()const;
private:
    int nThreads;
    int spwx, spwy;
    bool makeRgb; //If this is false we output XYZ color
};

inline void ParallelRenderer::setOutputRGB(bool rgb){ makeRgb = rgb; }
inline void ParallelRenderer::setOutputXYZ(bool xyz){ makeRgb = !xyz; }
inline bool ParallelRenderer::isOutputRGB()const{ return makeRgb; }
inline bool ParallelRenderer::isOutputXYZ()const{ return !makeRgb; }


#endif //PARALLEL_RENDERER_H
