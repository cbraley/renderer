#ifndef TEST_RNG_H
#define TEST_RNG_H

#include <UnitTest++.h>
//--
#include <iostream>
#include <cfloat>
#include <algorithm>
//--
#include "utils/RNG.h"

SUITE(RNG){

    //Test the range of the RNG over many iterations and make sure its reasonable
    TEST(RNGBasic){
        RNG rng1;
        RNG rng2;
        const int NUM_TESTS = 100;
        float roc1range[2] = {-FLT_MAX, FLT_MAX};
        float roc2range[2] = {-FLT_MAX, FLT_MAX};
        float roo1range[2] = {-FLT_MAX, FLT_MAX};
        float roo2range[2] = {-FLT_MAX, FLT_MAX};
        for(int i = 0; i < NUM_TESTS; i++){
            float roc1 = rng1.randomFloatOC();
            float roc2 = rng2.randomFloatOC();
            float roo1 = rng1.randomFloatOO();
            float roo2 = rng2.randomFloatOO();

            roc1range[0] = std::max<float>(roc1range[0], roc1);
            roc1range[1] = std::min<float>(roc1range[1], roc1);

            roc2range[0] = std::max<float>(roc2range[0], roc2);
            roc2range[1] = std::min<float>(roc2range[1], roc2);

            roo1range[0] = std::max<float>(roo1range[0], roo1);
            roo1range[1] = std::min<float>(roo1range[1], roo1);

            roo2range[0] = std::max<float>(roo2range[0], roo2);
            roo2range[1] = std::min<float>(roo2range[1], roo2);
        }

        CHECK(roc1range[0] > 0.0f && roc1range[1] <= 1.0f);
        CHECK(roo1range[0] > 0.0f && roo1range[1] <  1.0f);
        CHECK(roc2range[0] > 0.0f && roc2range[1] <= 1.0f);
        CHECK(roo2range[0] > 0.0f && roo2range[1] <  1.0f);

        CHECK(roc1range[0] != roc1range[1]);
        CHECK(roc2range[0] != roc2range[1]);
        CHECK(roo1range[0] != roo1range[1]);
        CHECK(roo2range[0] != roo2range[1]);
    }
}

#endif //TEST_BB_H
