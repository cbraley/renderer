#include "utils/RNGs.h"
//--
/*
#include <ctime>
//--
#include "utils/mtrand.h"
#include "utils/Assert.h"

static bool rngInitialized = false;
static MTRand mt; //Mersenne twisiter RNG
//the MTRand type generates doubles in [0,1)

static const unsigned long s = 89UL;
static const unsigned long array[] = {0x123, 0x234, 0x345, 0x456};

void RNG::initializeRNGs(bool useTimeSeed){
    if(useTimeSeed){
        unsigned long seed = (unsigned long) time(NULL);
        mt.seed(seed);
    }else{
        mt.seed(array, 4);
    }
    rngInitialized = true;
}

float RNG::randomFloat(const float minIncl, const float maxExcl){
    Assert(rngInitialized);
    Assert(maxExcl >= minIncl);
    return (float(mt()) * (maxExcl - minIncl)) + minIncl;
}

int RNG::randomInt(const int minIncl, const int maxExcl){
    Assert(rngInitialized);
    Assert(maxExcl >= minIncl);
    const double dres = ((mt()) * ( double(maxExcl) - double(minIncl))) + double(minIncl);
    return (int)dres;
}


double RNG::randomDouble(const double minIncl, const double maxExcl){
    Assert(rngInitialized);
    Assert(maxExcl >= minIncl);
    return (     (mt()) * (maxExcl - minIncl)) + minIncl;
}
*/
