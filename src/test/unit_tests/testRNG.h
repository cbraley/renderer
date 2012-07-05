#ifndef TEST_RNG_H
#define TEST_RNG_H

#include <UnitTest++.h>
//--
#include <iostream>
#include <cfloat>
#include <algorithm>
//--
#include "utils/RNG.h"
#include "third_party/tinymt32.h"

SUITE(RNG){

    static const int NUM_TESTS = 100000000; // 100 mil

    TEST(RawRNG){
        tinymt32_t rng;
        uint32_t seed = 2324235;
        tinymt32_init(&rng, seed);
        for(int i = 0; i < NUM_TESTS; i++){
            const float randomNum = 
                tinymt32_generate_float01(&rng);
            CHECK(randomNum >= 0.0f && randomNum < 1.0f);
            if(!(randomNum >= 0.0f && randomNum < 1.0f)){
                std::cerr << "The offending number was; " << randomNum << 
                    " on iteration: " << i << std::endl;
            }
        }
    }

    TEST(RNGSimple){
        RNG rng;
        for(int i = 0; i < NUM_TESTS; i++){
            const float randomNum = rng.randomFloatCO();
            CHECK(randomNum >= 0.0f && randomNum < 1.0f);
            if(!(randomNum >= 0.0f && randomNum < 1.0f)){
                std::cerr << "The offending number was; " << randomNum << 
                    " on iteration: " << i << std::endl;
            }
        }
    }

    //Test that the RNGs produce floats in the desired range
    TEST(RNGFloatRanges){
        RNG rng1;
        RNG rng2;
        for(int i = 0; i < NUM_TESTS; i++){
            const float r1CO = rng1.randomFloatCO();
//            const float r1OC = rng1.randomFloatOC();
            const float r1OO = rng1.randomFloatOO();

            const float r2CO = rng2.randomFloatCO();
//            const float r2OC = rng2.randomFloatOC();
            const float r2OO = rng2.randomFloatOO();

            CHECK(r1CO >= 0.0f && r1CO <  1.0f);
//            CHECK(r1OC >  0.0f && r1OC <= 1.0f);
            CHECK(r1OO >  0.0f && r1OO <  1.0f);
            
            CHECK(r2CO >= 0.0f && r2CO <  1.0f);
//            CHECK(r2OC >  0.0f && r2OC <= 1.0f);
            CHECK(r2OO >  0.0f && r2OO <  1.0f);
        }
    }

    TEST(RNGIntRange){
        RNG rng;

        const int MIN_INT = -9;
        const int MAX_INT =  16;

        const int NUMEL = MAX_INT - MIN_INT;
        bool arr[NUMEL];
        for(int i = 0; i < NUMEL; i++){
            arr[i] = false;
        }
        for(int i = 0; i < NUM_TESTS; i++){
            const int val = rng.randomIntCO(MIN_INT, MAX_INT);
            CHECK(val >= MIN_INT && val < MAX_INT);
            arr[val - MIN_INT] = true;
        }
        for(int i = 0; i < NUMEL; i++){
            CHECK(arr[i]);
        }
    }
}

#endif //TEST_BB_H
