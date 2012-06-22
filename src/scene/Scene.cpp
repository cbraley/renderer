#include "scene/Scene.h"
//--
#include "utils/Assert.h"

void Scene::computeBound(){
    size_t M = accel->getNumShapes();
    BoundingBox bbox;
    for(size_t i = 0; i < M; i++){
        bbox.Union(accel->getShape(i)->worldSpaceBound());
    }
    sceneBound = bbox;
}

Scene::Scene(AccelStructure* sceneData, const std::vector<Light*>& lightSources) :
    accel(sceneData), lights(lightSources)
{
    Assert(sceneData != NULL);
    for(size_t i = 0; i < lights.size(); i++){
        Assert(lights[i] != NULL);
    }
    computeBound();
}

Scene::~Scene(){
    delete accel; accel = NULL;
    for(size_t i = 0; i < lights.size(); i++){
        delete lights[i]; lights[i] = NULL;
    }
}

const AccelStructure* Scene::getSceneGeom()const{
    return accel;
}

Light* Scene::sampleLightsBasedOnPower()const{
    Assert(false);
    //TODO: Implement
    //Build a 1D CDF and go for it
    return NULL;
}

Light* Scene::sampleLightsUniform()const{
    Assert(false);
    //TODO: Implement
    return NULL;
//    return lights[RNG::randomInt(0, lights.size())];
}


BoundingBox Scene::getSceneBoundWS()const{
    return sceneBound;
}

Light* Scene::getLight(size_t idx)const{
    Assert(idx < lights.size());
    return lights[idx];
}
