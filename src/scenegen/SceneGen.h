#ifndef SCENE_GEN_H
#define SCENE_GEN_H

#include <vector>
//--
#include "math/Point.h"
#include "materials/Material.h"
#include "materials/PhongMaterial.h"
#include "primitives/Shape.h"
#include "cameras/Camera.h"
#include "lights/Light.h"


/**
 *  Code for proceduarally generating a few test scenes.
 *  Will likely be removed from the final system.
 */
namespace SceneGen{

    /**
     *  Generate the classic cornell box scene using the original measured data from here: http://www.graphics.cornell.edu/online/box/data.html
     *  Currently this data is hardcoded, but eventually I will want to use some type of scene description file. 
     */
    Camera* genCornellBox(std::vector<Shape*>& geom, std::vector<Light*>& lights);


    //Generate N random Phong materials that are green, blue, red and yellow with various material parameters.
    std::vector<PhongMaterial> genRandomMaterials(int N);

    //Make a sphereflake.
    //    N = recursion depth
    //    initScene = Initial scene data.
    //    center = center location of level 0 sphere
    //    rad = initial sphere radius
    //    mats = list of random materials, each spheres material is randomly chosen from this.
    //    rotY = How many degrees to rotate along y axis?
    std::vector<Shape*> genSphereFlake(int N, const std::vector<Shape*>& initScene, const Point& center, float rad, const std::vector<PhongMaterial>& mats, float rotY);

    //Simple scene with 4 spheres
    std::vector<Shape*> genSceneSpheres();

    std::vector<Shape*> genTriangleScene();


}


#endif //SCENE_GEN_H
