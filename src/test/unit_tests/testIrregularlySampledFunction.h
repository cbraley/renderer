#ifndef TEST_IRREGULARLY_SAMPLED_FUNCTION_H
#define TEST_IRREGULARLY_SAMPLED_FUNCTION_H

#include <algorithm>
#include <vector>
//--
#include <UnitTest++.h>
//--
#include "math/IrregularlySampledFunction.h"
#include "math/MathUtils.h"

SUITE(IrregularlySampledFunctionTests){


    inline float randomFloat(float a, float b){
        return ((b-a)*((float)rand()/RAND_MAX))+a;
    }

    // Create a function with samples at a few points.
    // Make sure that the interpolation routiens return the exact
    // function values at these points.
    TEST(IrregularlySampledFunctionInterpolateAtNodePoints){
        float x[] = {1.0f  , 10.0f , -20.0f, 100.0f, 200.0f};
        float y[] = {-12.0f, -13.0f, 100.0f, 50.0f , 0.0f  };

        std::vector<float> xs(x, x + sizeof(x) / sizeof(float));
        std::vector<float> ys(y, y + sizeof(y) / sizeof(float));

        IrregularlySampledFunction func(xs, ys);
        std::cout << func << std::endl;

        // Check that we match at the sample points
        for(size_t i = 0; i < xs.size(); i++){
            const float corrX = xs[i];
            const float corrY = ys[i];
            const float interpolatedVal = func(corrX);
            CHECK_EQUAL(corrY, interpolatedVal);
        }
    }

    TEST(TestLinearInterpolation){
        //TODO
    }

    // Make sure that, regardless of constructing the function via "addSample"
    // we get the same results as if we had constructed the whole thing explicitly.
    TEST(TestAddSamplesWorksSameAsConstructor){
        //Make the reference function to compare to
        float x[] = {1.0f  , 10.0f , -20.0f, 100.0f, 200.0f};
        float y[] = {-12.0f, -13.0f, 100.0f, 50.0f , 0.0f  };
        std::vector<float> xs(x, x + sizeof(x) / sizeof(float));
        std::vector<float> ys(y, y + sizeof(y) / sizeof(float));
        IrregularlySampledFunction referenceFunc(xs, ys);

        //...and make an array of pair<float,float>, where each pair is a sample
        std::vector< std::pair<float,float> > pairList;
        for(size_t i = 0; i < sizeof(x)/sizeof(float); i++){
            std::pair<float,float> currPair = std::make_pair(x[i],y[i]);
            pairList.push_back(currPair);
        }

        //Do NUM_TRIALS trials where we construct the function using addSamples() each
        //new loop iteration.  We shuffle the pairList each loop.
        const int NUM_TRIALS = 10;
        const int NUM_COMPARE_POINTS = 5;
        for(int i = 0; i < NUM_TRIALS; i++){

            IrregularlySampledFunction newFunc; //Create a function with no samples

            //Shuffle the sample points and add all the samples in random order
            std::random_shuffle(pairList.begin(), pairList.end());
            for(size_t j = 0; j < pairList.size(); ++j){
                bool nonDup = newFunc.addSample(pairList[j].first, pairList[j].second);
                CHECK(nonDup);
            }
            CHECK_EQUAL(referenceFunc.getNumSamples(), newFunc.getNumSamples());

            //Make sure the functions match at many random points
            for(int h = 0; h < NUM_COMPARE_POINTS; ++h){
                const float randomX = randomFloat(-25.0f, 205.0f);
                CHECK_EQUAL(referenceFunc(randomX), newFunc(randomX));
            }
            //...and at the sample points
            for(size_t k = 0; k < pairList.size(); ++k){
                CHECK_EQUAL(referenceFunc[k], newFunc[k]);
            }
        }
    }


}

#endif //TEST_IRREGULARLY_SAMPLED_FUNCTION_H
