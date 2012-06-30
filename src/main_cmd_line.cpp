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


//Scene variables
static Transform camToWorld;
static float fov   = 20.0f;
static float zNear = 0.0f;
static float zFar  = Constants::MAX_FLOAT_VAL;
static int spp     = 300;


int main(int argc, char** argv){

    //Read command line params
    if(argc < 7){
        std::cerr << "Usage: " << std::endl << argv[0] <<
            " camX camY camZ width height outname.png rot(optional) num_bounces(optional)" << std::endl;
        return 0;
    }
    float camX = atof(argv[1]);
    float camY = atof(argv[2]);
    float camZ = atof(argv[3]);
    int width  = atoi(argv[4]);
    int height = atoi(argv[5]);
    std::cout << "Rendering a: " << width << " by " << height << " image." << std::endl;
    std::string outName(argv[6]);
    float rot = 0.0f;
    if(argc >= 8){ rot = atof(argv[7]); }
    int numBounces = 3;
    if(argc >= 9){ numBounces = atof(argv[8]); }
    camToWorld = Transform::translate(Vector(camX, camY, camZ));

    //Generate scene
    std::vector<Shape*> shapes;
    std::vector<Light*> lights;
    Camera* cam = SceneGen::genCornellBox(shapes, lights);
    std::cout << *cam << std::endl;

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
