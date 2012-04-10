#ifndef RNG_H
#define RNG_H

#include <ctime>
#include "tinymt32.h"


/**
 *  Random number generator.  Creates random numbers over  
 *  a half-open interval [begin, end) where end > begin.
 *
 *  Inspired by, and highly similar to, random number generation
 *  in PBRT 2 by Pharr and Humphreys.
 *
 *  Internally uses a Mersenne twister implementation originally from 
 *  Takuji Nishimura and Makoto Matsumoto that was later ported to C++ by
 *  Jasper Bedaux.  See http://www.bedaux.net/mtrand/
 * 
 *  This code can be used "freely," see: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/elicense.html
 */
class RNG{
public:
    /**
     *  Initialize an RNG with a predetermined seed of the caller's choice.
     */
    RNG(unsigned int seedVal);
   
    /**
     *  Initialize the RNG.  Defaults to using a seed based on the clock,
     *  otherwise use a seed from a default array.
     */
    RNG(bool useTemporalSeed = true);

    /*
     *  Generate a random floating point number in the 
     *  range (0,1].
     */
    float randomFloatOC()const;


    /*
     *  Generate a random floating point number in the 
     *  range (0,1).
     */
    float randomFloatOO()const;

    int randomIntOC(int startIncl, int endExcl)const;
    float randomFloatOC(float startInc, float endIncl)const;
private:
    mutable tinymt32_t rng;

    //No copying!
    RNG& operator=(const RNG& other)const;
    RNG(const RNG& other);
};

inline float RNG::randomFloatOC(float startInc, float endIncl)const{
    return 7.0f; //TODO
}

inline int RNG::randomIntOC(int startIncl, int endExcl)const{
    return 2; //TODO: DO
}


inline RNG::RNG(unsigned int seedVal){
    tinymt32_init(&rng, seedVal);
}

inline RNG::RNG(bool useTemporalSeed){
    const uint32_t SEED = 16541;
    if(useTemporalSeed){

        //Get a random clock_t (usually an unsigned long)
        //and interpret the first 32 bits of it as a seed
        //for the RNG
        union{
            clock_t theTime;
            uint32_t temporalSeed;
        };
        theTime = clock();
        tinymt32_init(&rng, temporalSeed);
        
    }else{
        tinymt32_init(&rng, SEED);
    }
}

inline float RNG::randomFloatOO()const{
    return tinymt32_generate_floatOO(&rng);
}
inline float RNG::randomFloatOC()const{
    return tinymt32_generate_floatOC(&rng);
}





#endif //RNG_H
