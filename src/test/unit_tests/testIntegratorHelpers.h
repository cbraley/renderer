#ifndef TEST_INTEGRATOR_HELPERS_H
#define TEST_INTEGRATOR_HELPERS_H

#include <UnitTest++.h>
//--
#include <iostream>
//--
#include "integrators/IntegratorHelpers.h"
#include "scene/Scene.h"
#include "lights/PointLight.h"
#include "color/Spectrum.h"
#include "utils/RNG.h"
#include "accel/NoAccelStructure.h"

SUITE(IntegratorHelpers){

    inline float genUniformRand(RNG* rng){
        return rng->randomFloatCO();
    }

    //generate a test scene, must be deleted by the caller
    Scene* genTestScene(){
        std::vector<Light*> lights;

        const float MIN_NM = 400.0f;
        const float MAX_NM = 720.0f;
        int counter = 0;
        lights.push_back( new PointLight( Spectrum(2.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(1.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(0.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(3.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(0.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(0.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(4.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(20.0f, MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(10.0f, MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );
        lights.push_back( new PointLight( Spectrum(3.0f , MIN_NM + ((float)counter++), MAX_NM) , Transform() ) );

        AccelStructure* accel = new NoAccelStructure(); 
        Scene* s = new Scene(accel, lights);
        return s;
    }

    TEST(LightSamplingPowerHeuristic){
    }

    TEST(LightSamplingUniform){
    }

}

#endif //TEST_INTEGRATOR_HELPERS_H
