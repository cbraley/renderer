#include <string>
#include <iostream>
#include <algorithm>
//--
#include "utils/Constants.h"
#include "renderers/Renderer.h"
#include "color/SpectrumPlotter.h"
#include "primitives/Sphere.h"

#include "integrators/IntersectionCheckIntegrator.h"
#include "integrators/DirectLightIntegrator.h"

#include "math/Normal.h"

#include "primitives/Shape.h"
#include "utils/RNGs.h"
#include "accel/NoAccelStructure.h"
#include "color/Spectrum.h"
#include "lights/PointLight.h"
#include "imsampling/ImageSampler.h"
#include "imsampling/JitteredSampler.h"
#include "imsampling/BoxFilter.h"
#include "imsampling/GaussianFilter.h"
#include "imsampling/TriangleFilter.h"
#include "scenegen/SceneGen.h"
#include "imageio/ToneMap.h"
#include "utils/StringUtils.h"
#include "primitives/Quad.h"
#include "primitives/Triangle.h"
#include "scene/Scene.h"

#include "renderers/ParallelRenderer.h"

#include "imageio/ImageIO.h"

//Scene variables
static Transform camToWorld;
static float fov   = 20.0f;
static float zNear = 0.0f;
static float zFar  = MAX_FLOAT_VAL;
static int spp     = 300;

//Constants
static const Point SCENE_CENTER(0.0f, 0.0f, 10.0f);
static const int NUM_MATERIALS = 30;

//Functions for spectrum creation
float red  (float nm){ return nm > 650.0f ? 1.0f : 0.0f;                  }
float green(float nm){ return (nm > 500.0f && nm < 600.0f) ? 1.0f : 0.0f; }
float blue (float nm){ return nm < 500.0f ? 1.0f : 0.0f;                  }

//p = Location, div = amount by which to divide light spectrum
std::vector<Light*> createLightAtPoint(const Point& p, float div){
    std::vector<Light*> lights;
    lights.push_back(
        new PointLight(Spectrum(Spectrum::CIE_ILLUM_E) / div,
        Transform::translate( Vector(p.x, p.y, p.z) ) ) );
    return lights;
}


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

    //Setup geometry
    std::cout << "Creating scene..." << std::endl;

    //Scene
    std::vector<Shape*> shapes;           //Geometry
    std::vector<PhongMaterial> materials; //Materials to randomly assign to objects
    std::vector<Light*> lights;           //Point sources
    Camera* cam = NULL;                   //Eye


    //Generate 3 random RGB materials
    const Spectrum R_SPEC(red     ,400.0f,720.0f,10.0f);
    const Spectrum G_SPEC(green   ,400.0f,720.0f,10.0f);
    const Spectrum B_SPEC(blue    ,400.0f,720.0f,10.0f);
    PhongMaterial Rs (R_SPEC , Spectrum(Spectrum::CIE_ILLUM_E) / 100.0f, 10.0f, 0.1f);
    PhongMaterial Gs (G_SPEC , Spectrum(Spectrum::CIE_ILLUM_E) / 100.0f, 10.0f, 0.1f);
    PhongMaterial Bs (B_SPEC , Spectrum(Spectrum::CIE_ILLUM_E) / 100.0f, 10.0f, 0.1f);
    materials.push_back(Rs);
    materials.push_back(Gs);
    materials.push_back(Bs);


    //Sphere flake
    bool SPHERE_FLAKE   = false;
    bool CORNELL_BOX    = true;
    bool SIMPLE_SPHERES = false;
    if(SPHERE_FLAKE){
        std::cout << "Sphere Flake." << std::endl;
        shapes = SceneGen::genSphereFlake(3, shapes, Point(0.0f,0.0f,0.0f), 1.0f, materials, rot);
        lights = createLightAtPoint(Point(0.0f, 0.0f, 0.0f), 100.0f);
        //cam = new Camera(camToWorld, fov, zNear, zFar, width, height);
        cam = NULL;
    }else if(CORNELL_BOX){
        std::cout << "Cornell Box." << std::endl;
        cam = SceneGen::genCornellBox(shapes, lights);
        std::cout << *cam << std::endl;
    }else if(SIMPLE_SPHERES){
//        std::cout << "Spheres." << std::endl;
//        shapes = SceneGen::genSceneSpheres();
        shapes = SceneGen::genTriangleScene();
        lights = createLightAtPoint(Point(0.0f, 0.0f, 0.0f), 100.0f);
        //cam = new Camera(camToWorld, fov, zNear, zFar, width, height);
        cam = NULL;
    }else{
        return 1;
    }
    std::cout << "Done creating scene." << std::endl;

    //Setup things needed by renderer
    const float pixelWidth  = 1.0f;
    const float pixelHeight = 1.0f;
    ImageSampler* sampler = new JitteredSampler(width, height, spp,
        pixelWidth / 2.0f, pixelHeight / 2.0f); //Jitter x and jitter Y

    ImageSensor* ccd = new ImageSensor(width, height, NULL ,
        //new BoxFilter(pixelWidth, pixelHeight)
        new GaussianFilter(3.0f, pixelWidth, pixelHeight)
    );
    AccelStructure* accelScene = new NoAccelStructure();
    accelScene->buildAccelStructure(shapes);
    Scene* scene = new Scene(accelScene, lights);
    Integrator* integrator = new DirectLightIntegrator();
//    Integrator* integrator = new WhittedIntegrator();

    //Render a picture
    std::cout << "Rendering scene..." << std::endl;
//    Renderer* rt = new Renderer();
    Renderer* rt = new ParallelRenderer(32, 32, 4);
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

        bool ok = ImageIO::writePFM(outName, im, "HDR pfm from colins command line renderer");
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
