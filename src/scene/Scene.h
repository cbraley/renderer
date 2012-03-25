#ifndef SCENE_H
#define SCENE_H

#include <vector>
//--
#include "BoundingBox.h"
#include "Light.h"
#include "Ray.h"
#include "AccelStructure.h"
#include "RaySurfIntersection.h"

class Scene{
public:

    Scene(AccelStructure* sceneData, const std::vector<Light*>& lightSources);

    virtual ~Scene();

    const AccelStructure* getSceneGeom()const;

    Light* sampleLightsBasedOnPower()const;

    Light* sampleLightsUniform()const;

    BoundingBox getSceneBoundWS()const;

    inline size_t getNumLights()const { return lights.size(); }

    Light* getLight(size_t idx)const;

private:

    //Non-copyable
    Scene(const Scene& other);
    Scene& operator=(const Scene& rhs);
    Scene();

    //Data members
    AccelStructure* accel;
    std::vector<Light*> lights;
    BoundingBox sceneBound;

    void computeBound();

};


#endif //SCENE_H
