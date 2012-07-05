#include "scenegen/SceneGen.h"
//--
#include "color/Spectrum.h"
#include "utils/RNG.h"
#include "primitives/Sphere.h"
#include "math/MathUtils.h"
#include "primitives/Quad.h"
#include "color/Spectrum.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"
#include "primitives/Triangle.h"
#include "materials/LambertianBRDF.h"

namespace SceneGen{


Camera* genCornellBox(std::vector<Shape*>& geom, std::vector<Light*>& lights){

    //Lights
    /*
    float LIGHT_SPECTRUM[4] = { //The actual light
        0.0f,  //400 nm
        8.0f,  //500 nm
        15.6f, //600 nm
        18.4f  //700 nm
    };
    */

    float LIGHT_SPECTRUM[4] = { //White light
        1.0f,  //400 nm
        1.0f,  //500 nm
        1.0f,  //600 nm
        1.0f   //700 nm
    };

    Point lq1(343.0f,   548.8f, 227.0f);
    Point lq2(343.0f,   548.8f, 332.0f);
    Point lq3(213.0f,   548.8f, 332.0f);
    Point lq4(213.0f,   548.8f, 227.0f);
    Shape* lightQuad = new Quad(lq1, lq2, lq3, lq4); //Light geometry
    const Spectrum lightSPD(&(LIGHT_SPECTRUM[0]), 400.0f, 700.0f, 100.0f, 4);


    //const float Tkelvin = 5000.0f;
    //const float minNmVal = 400.0f;
    //const float maxNmVal = 720.0f;
    //const int numSamples = 33;
    //std::cout << "Light color temperature  = " << Tkelvin << "K" << std::endl;
    //const Spectrum lightSPD(Tkelvin, minNmVal, maxNmVal, numSamples);
    //const Spectrum lightSPD(Spectrum::CIE_ILLUM_F_12);

    //Vector lcenter(213.0f,   548.8f, 227.0f);
    //Transform lightTransform = Transform::translate(lcenter);
    //lights.push_back(new PointLight(lightSPD, lightTransform));

    Light* al = new AreaLight(lightSPD, (Quad*)lightQuad);
    lightQuad->setEmitter(al);
    lights.push_back(al);


    //Materials
    //Wavelength  white   green   red
    const double MAT_DATA[77][4] = {
        {400,  0.343,  0.092,  0.040},
        {404,  0.445,  0.096,  0.046},
        {408,  0.551,  0.098,  0.048},
        {412,  0.624,  0.097,  0.053},
        {416,  0.665,  0.098,  0.049},
        {420,  0.687,  0.095,  0.050},
        {424,  0.708,  0.095,  0.053},
        {428,  0.723,  0.097,  0.055},
        {432,  0.715,  0.095,  0.057},
        {436,  0.710,  0.094,  0.056},
        {440,  0.745,  0.097,  0.059},
        {444,  0.758,  0.098,  0.057},
        {448,  0.739,  0.096,  0.061},
        {452,  0.767,  0.101,  0.061},
        {456,  0.777,  0.103,  0.060},
        {460,  0.765,  0.104,  0.062},
        {464,  0.751,  0.107,  0.062},
        {468,  0.745,  0.109,  0.062},
        {472,  0.748,  0.112,  0.061},
        {476,  0.729,  0.115,  0.062},
        {480,  0.745,  0.125,  0.060},
        {484,  0.757,  0.140,  0.059},
        {488,  0.753,  0.160,  0.057},
        {492,  0.750,  0.187,  0.058},
        {496,  0.746,  0.229,  0.058},
        {500,  0.747,  0.285,  0.058},
        {504,  0.735,  0.343,  0.056},
        {508,  0.732,  0.390,  0.055},
        {512,  0.739,  0.435,  0.056},
        {516,  0.734,  0.464,  0.059},
        {520,  0.725,  0.472,  0.057},
        {524,  0.721,  0.476,  0.055},
        {528,  0.733,  0.481,  0.059},
        {532,  0.725,  0.462,  0.059},
        {536,  0.732,  0.447,  0.058},
        {540,  0.743,  0.441,  0.059},
        {544,  0.744,  0.426,  0.061},
        {548,  0.748,  0.406,  0.061},
        {552,  0.728,  0.373,  0.063},
        {556,  0.716,  0.347,  0.063},
        {560,  0.733,  0.337,  0.067},
        {564,  0.726,  0.314,  0.068},
        {568,  0.713,  0.285,  0.072},
        {572,  0.740,  0.277,  0.080},
        {576,  0.754,  0.266,  0.090},
        {580,  0.764,  0.250,  0.099},
        {584,  0.752,  0.230,  0.124},
        {588,  0.736,  0.207,  0.154},
        {592,  0.734,  0.186,  0.192},
        {596,  0.741,  0.171,  0.255},
        {600,  0.740,  0.160,  0.287},
        {604,  0.732,  0.148,  0.349},
        {608,  0.745,  0.141,  0.402},
        {612,  0.755,  0.136,  0.443},
        {616,  0.751,  0.130,  0.487},
        {620,  0.744,  0.126,  0.513},
        {624,  0.731,  0.123,  0.558},
        {628,  0.733,  0.121,  0.584},
        {632,  0.744,  0.122,  0.620},
        {636,  0.731,  0.119,  0.606},
        {640,  0.712,  0.114,  0.609},
        {644,  0.708,  0.115,  0.651},
        {648,  0.729,  0.117,  0.612},
        {652,  0.730,  0.117,  0.610},
        {656,  0.727,  0.118,  0.650},
        {660,  0.707,  0.120,  0.638},
        {664,  0.703,  0.122,  0.627},
        {668,  0.729,  0.128,  0.620},
        {672,  0.750,  0.132,  0.630},
        {676,  0.760,  0.139,  0.628},
        {680,  0.751,  0.144,  0.642},
        {684,  0.739,  0.146,  0.639},
        {688,  0.724,  0.150,  0.657},
        {692,  0.730,  0.152,  0.639},
        {696,  0.740,  0.157,  0.635},
        {700,  0.737,  0.159,  0.642}
    };
    float WHITE_SPECTRAL_REFL[77];
    float GREEN_SPECTRAL_REFL[77];
    float RED_SPECTRAL_REFL  [77];
    for(int i = 0; i < 77; i++){
        WHITE_SPECTRAL_REFL[i] = (float) (MAT_DATA[i][1]);
        GREEN_SPECTRAL_REFL[i] = (float) (MAT_DATA[i][2]);
        RED_SPECTRAL_REFL  [i] = (float) (MAT_DATA[i][3]);
    }
    const Spectrum R_CORNELL(&(RED_SPECTRAL_REFL[0])  , 400.0f, 700.0f, 4.0f, 77);
    const Spectrum G_CORNELL(&(GREEN_SPECTRAL_REFL[0]), 400.0f, 700.0f, 4.0f, 77);
    const Spectrum W_CORNELL(&(WHITE_SPECTRAL_REFL[0]), 400.0f, 700.0f, 4.0f, 77);

    //Geometry
    //    bw = back wall
    Point bw1(549.6f,   0.0f  , 559.2f);
    Point bw2(0.0f  ,   0.0f  , 559.2f);
    Point bw3(0.0f  ,   548.8f, 559.2f);
    Point bw4(556.0f,   548.8f, 559.2f);
    //    rw = right wall
    Point rw1(0.0f  , 0.0f   , 559.2f );
    Point rw2(0.0f  , 0.0f   , 0.2f   );
    Point rw3(0.0f  , 548.0f , 0.2f   );
    Point rw4(0.0f  , 548.0f , 559.2f );
    //    lw = left wall
    Point lw1(552.7f,  0.0f  ,0.0f    );
    Point lw2(549.6f,  0.0f  , 559.2f );
    Point lw3(556.0f, 548.0f , 559.2f );
    Point lw4(556.0f, 548.8f ,0.0f    );
    //    fw = floor wall
    Point fw1(552.8f, 0.0f   , 0.0f   );
    Point fw2(0.0f  , 0.0f   , 0.0f   );
    Point fw3(0.0f  , 0.0f   , 559.2f );
    Point fw4(549.6f, 0.0f   , 559.2f );
    //    cw = ceiling wall
    //const float wall_y = 548.8f;
    const float wall_y = 570.0f;
    Point cw1(556.0f, wall_y , 0.0f   );
    Point cw2(556.0f, wall_y , 559.2f );
    Point cw3(0.0f  , wall_y , 559.2f );
    Point cw4(0.0f  , wall_y , 0.0    );


    //Add the geometry to the scene
    geom.push_back( new Quad(bw1, bw2, bw3, bw4) ); //Back wall   (white)
    geom.push_back( new Quad(rw1, rw2, rw3, rw4) ); //Right wall  (green)
    geom.push_back( new Quad(lw1, lw2, lw3, lw4) ); //Left wall   (red)
    geom.push_back( new Quad(fw1, fw2, fw3, fw4) ); //Floor       (white)
    geom.push_back( new Quad(cw1, cw2, cw3, cw4) ); //Ceiling     (white)
    geom.push_back( lightQuad                    ); //Light quad  (white)

    geom.push_back(new Sphere(
        Transform::translate(
            Vector(250.0f, 150.0f, 250.0f)
        ),
            75.0f
        ));

    //Add the materials
    int gIdx = 0;
    geom[gIdx++]->setBRDF( new LambertianBRDF(W_CORNELL) );
    geom[gIdx++]->setBRDF( new LambertianBRDF(G_CORNELL) );
    geom[gIdx++]->setBRDF( new LambertianBRDF(R_CORNELL) );
    geom[gIdx++]->setBRDF( new LambertianBRDF(W_CORNELL) );
    geom[gIdx++]->setBRDF( new LambertianBRDF(W_CORNELL) );
    geom[gIdx++]->setBRDF( new LambertianBRDF(W_CORNELL) );

    geom[gIdx++]->setBRDF(new LambertianBRDF(G_CORNELL + R_CORNELL));

    //Camera
    Vector camPosVec(278.0f, 243.0f, -800.0f);
    Transform camTrans = Transform::translate(camPosVec);
    return new Camera(camTrans,
        0.035f, //focal length
        0.025f, 0.025f); //Image plane width, height
}




static int cc = 0;
std::vector<Shape*> genSphereFlake(int N, const std::vector<Shape*>& initScene, const Point& center, float rad, const std::vector<PhongMaterial>& mats, float rotY){
    std::vector<Shape*> s = initScene;

    //Base case, add a plane in the back
    if(N == 0){
        if(cc++ == 0){
            /*
            s.push_back(
                new Plane(
                Transform::translate(Vector(0.0f,0.0f,30.0f))  *
                Transform::rotateX(90.0f)
                )
            );
            s[s.size()-1]->setMaterial(new PhongMaterial());
            */
        }
        return s;
    }

    RNG rng;

    //Recursive case


    //Make new sphere with random material
    Shape* c = new Sphere(Transform::translate(Vector(center.x, center.y, center.z)), rad);
    const int randIdx = rng.randomIntCO(0, (int) mats.size());
    //c->setMaterial( new PhongMaterial(mats[randIdx]) );
    s.push_back(c);

    //Compute parameters for recursive calls
    const float newRad = rad * 0.5f;
    const float move   = rad + newRad;
    Ray rlst[6] = {
        Ray(center, Vector( 1.0f, 0.0f, 0.0f), true),
        Ray(center, Vector( 0.0f, 1.0f, 0.0f), true),
        Ray(center, Vector( 0.0f, 0.0f, 1.0f), true),
        Ray(center, Vector(-1.0f, 0.0f, 0.0f), true),
        Ray(center, Vector( 0.0f,-1.0f, 0.0f), true),
        Ray(center, Vector( 0.0f, 0.0f,-1.0f), true)
    };
    const Transform tf = Transform::rotateY(rotY);
    for(int i = 0; i < 6; i++){
        rlst[i] = Ray(rlst[i].getOrigin(), tf(rlst[i].getDir()));
    }

    //Make all 6 recursive calls
    for(int i = 0; i < 6; i++){
        const Ray r = rlst[i];
        const Point newCenter = r(move);
        s = SceneGen::genSphereFlake(N - 1, s, newCenter, newRad, mats, rotY);
    }
    return s;
}


std::vector<Shape*> genSceneSpheres(){
    std::vector<Shape*> shapes;
    shapes.push_back( new Sphere(  Transform::translate(Vector(-2.0f, 0.0f, 10.0f))  , 0.9f) );
    shapes.push_back( new Sphere(  Transform::translate(Vector( 0.0f, 0.0f, 10.0f))  , 0.9f) );
    shapes.push_back( new Sphere(  Transform::translate(Vector( 2.0f, 0.0f, 10.0f))  , 0.9f) );
    shapes.push_back( new Sphere(  Transform::translate(Vector( 1.0f, 0.5f, 5.0f))  , 0.25f) );

    const float amb = 0.00f;
    Spectrum specular1(1.0f, 400.0f, 720.0f);
    Spectrum specular2(1.0f, 400.0f, 720.0f);
    Spectrum specular3(1.0f, 400.0f, 720.0f);

    const float NM_STEP = 50.0f;
    std::vector<float> vals1, vals2, vals3;
    //Greenish             Blueish                Redish
    vals1.push_back(0.0f); vals2.push_back(0.0f); vals3.push_back(0.0f); //400
    vals1.push_back(0.1f); vals2.push_back(0.5f); vals3.push_back(0.0f); //450
    vals1.push_back(0.9f); vals2.push_back(0.1f); vals3.push_back(0.0f); //500
    vals1.push_back(0.0f); vals2.push_back(0.0f); vals3.push_back(0.1f); //550
    vals1.push_back(0.0f); vals2.push_back(0.0f); vals3.push_back(0.3f); //600
    vals1.push_back(0.0f); vals2.push_back(0.0f); vals3.push_back(0.5f); //650
    vals1.push_back(0.0f); vals2.push_back(0.0f); vals3.push_back(0.0f); //700


    Spectrum diffuse1(vals1, 400.0f, NM_STEP);
    Spectrum diffuse2(vals2, 400.0f, NM_STEP);
    Spectrum diffuse3(vals3, 400.0f, NM_STEP);

    std::cout << diffuse1 << std::endl;
    std::cout << diffuse2 << std::endl;
    std::cout << diffuse3 << std::endl;

    /*
    shapes[0]->setMaterial(new PhongMaterial(diffuse1, specular1, 100 , amb ));
    shapes[1]->setMaterial(new PhongMaterial(diffuse2, specular2, 5   , amb ));
    shapes[2]->setMaterial(new PhongMaterial(diffuse3, specular3, 50  , amb ));
    shapes[3]->setMaterial(new PhongMaterial(Spectrum(Spectrum::CIE_ILLUM_F_2) / 10.0f, specular1, 10.0f  , amb ));
    */

    return shapes;
}

std::vector<Shape*> genTriangleScene(){
    std::vector<Shape*> shapes;

    const float XW = 2.0f;
    for(float x = -10.0f; x <= 10.0f; x += XW){
        shapes.push_back(new Triangle(
            Point( x + 1.0f , 0.0f, 9.0f),
            Point( x + -1.0f, 0.0f, 9.0f),
            Point( x + 0.0f , 3.0f, 9.0f)  ));
        shapes.push_back(new Triangle(
            Point( x + 1.0f , 0.0f, 9.0f),
            Point( x + -1.0f, 0.0f, 9.0f),
            Point( x + 0.0f ,-3.0f, 9.0f)  ));
    }


    return shapes;
}

}
