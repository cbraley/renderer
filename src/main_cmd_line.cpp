#include <string>
#include <iostream>
#include <algorithm>
//--
/*
#include "color/SpectrumPlotter.h"
#include "primitives/Sphere.h"
#include "integrators/IntersectionCheckIntegrator.h"
#include "math/Normal.h"
#include "primitives/Shape.h"
#include "lights/PointLight.h"
#include "imsampling/TriangleFilter.h"
#include "primitives/Quad.h"
#include "primitives/Triangle.h"
*/
#include "scene/Scene.h"
#include "imageio/ToneMap.h"
#include "imageio/ImageIO.h"
#include "utils/StringUtils.h"
#include "color/Spectrum.h"
#include "integrators/DirectLightIntegrator.h"
#include "renderers/Renderer.h"
#include "renderers/ParallelRenderer.h"
#include "accel/NoAccelStructure.h"
#include "imsampling/JitteredSampler.h"
#include "imsampling/ImageSampler.h"
#include "imsampling/BoxFilter.h"
#include "scenegen/SceneGen.h"
#include "imsampling/GaussianFilter.h"
//--
#include "accel/BVH.h"

//TODO: Note that this file is all temporary code for now
//Right now I just have a hardcoded scene with a few parameters that can be adjusted via the command line
//In the future, I intend to have this application accept a scene file, rendering parameters, and an output
//file location on the command line


//Scene variables
static float fov   = 20.0f;
static float zNear = 0.0f;
static float zFar  = Constants::MAX_FLOAT_VAL;
static int spp     = 300;


int main(int argc, char** argv){

    //Usage message
    if(argc > 1 && strcmp(argv[1], "--help") == 0){
        std::cerr << "Usage(all parameters optional): " << std::endl << argv[0] <<
            " camX camY camZ imWidth imHeight outImage.png rot" << std::endl;
        return 0;
    }

    //Parse command line parameters
    int index = 1; //which arg are we currently parsing?
    float camX, camY, camZ, rotDeg;
    camX = camY = camZ = rotDeg = 0.0f;
    int width, height;
    width = height = 64;
    std::string outName("default.ppm");
    if(index < argc)
        camX = atof(argv[index++]);
    if(index < argc)
        camY = atof(argv[index++]);
    if(index < argc)
        camZ = atof(argv[index++]);

    if(index < argc)
        width  = atoi(argv[index++]);
    if(index < argc)
        height = atoi(argv[index++]);
    Assert(width > 0 && height > 0);
    if(index < argc)
        outName = std::string(argv[index++]);
    if(index < argc)
        rotDeg = atof(argv[index++]);
    
   
    //Construct camera placement transformation
    Transform moveTrans  = Transform::translate(Vector(camX, camY, camZ));
    Transform rotTrans   = Transform::rotateZ(rotDeg);
    Transform camToWorld = moveTrans * rotTrans;

    //Generate scene
    std::vector<Shape*> shapes;
    std::vector<Light*> lights;
    Camera* cam = SceneGen::genCornellBox(shapes, lights);

    //Setup image plane and sampler
    const float pixelWidth  = 1.0f;
    const float pixelHeight = 1.0f;
    ImageSampler* sampler = new JitteredSampler(width, height, spp,
        pixelWidth / 2.0f, pixelHeight / 2.0f); //Jitter x and jitter Y
    ImageSensor* ccd = new ImageSensor(width, height, NULL ,
        new GaussianFilter(3.0f, pixelWidth, pixelHeight)
    );

    //Acceleration structure
    AccelStructure* accelScene = new NoAccelStructure();
    accelScene->buildAccelStructure(shapes);

    //Full scene
    Scene* scene = new Scene(accelScene, lights);

    //Setup integrator and renderer
    Integrator* integrator = new DirectLightIntegrator();
    Renderer* rt = new Renderer();
    //Renderer* rt = new ParallelRenderer(32, 32, 4);

    //Render a picture
    std::cout << "Rendering scene..." << std::endl;
    HDRImage* im = rt->render(sampler, cam, ccd, scene, integrator);
    std::cout << "Done rendering scene." << std::endl;

    //Cleanup memory
    delete rt;
    delete sampler;
    delete cam;
    delete ccd;
    delete scene;
    delete integrator;


    //Potentially tonemap the image
    LDRImage* tmapim = NULL;
    if(StringUtils::endsWith(outName, ".pfm")){
        std::cout << "Skipping tone mapping and saving an HDR image directly..." << std::endl;

        bool ok = ImageIO::writePFM(outName, im);
        if(!ok){
            std::cerr << "ERROR - could not write image: " << outName << std::endl;
        }
    }else{

        std::cout << "Tone mapping..." << std::endl;
        tmapim = ToneMap::toneMapMaxToWhite(im, true);

        ToneMap::makeWhiteBorder(tmapim);
        std::cout << "Done tone mapping." << std::endl;

        //Save to disk
        std::cout << "Outputting image to " << outName << "..." << std::endl;
        bool ok = ImageIO::writePPM(outName, tmapim, "LDR ppm from colins command line renderer");
        if(!ok){
            std::cerr << "ERROR - could not write image: " << outName << std::endl;
        }
    }
    delete im; im = NULL;
    delete tmapim; tmapim = NULL;
    std::cout << "Done outputting image." << std::endl;

    return 0;
}
