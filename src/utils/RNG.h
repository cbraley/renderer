#ifndef RNG_H
#define RNG_H

#include <ctime>
//--
#include "third_party/tinymt32.h"
//--
#include "utils/Assert.h"

/**
 *  Note - I am experiencing problems with tinymt32!  The function:
 *  tinymt32_generate_float SHOULD return numbers in the range [0,1).
 *  However, it very occasionally returns 1.0f, thus making its range:
 *  [0,1].
 *
 *  When I use the function tinymt32_generate_float01 (which is supposed to 
 *  have the same functionality, but be slightly slower), I get correct results.
 *
 *  Note that I have slightly modified the tinymt32.h file to work around issues 
 *  with UINT32_C macros not being properly defined in g++.
 */
#define USE_UNION_TRICK //This MUST be defined for correct behavior!  See above!

/**
 *  Random number generator.  Can return uniform random numbers 
 *  over the following intervals: [0,1), (0,1], and (0,1).
 *      
 *  Note - This class is not thread-safe across multiple threads!
 *  It is REQUIRED to use one RNG per thread to avoid race conditions.
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
     *  Initialize the RNG.  Defaults to using a deterministic seed, but can be seeded by the clock
     *  as well.
     */
    RNG(bool useTemporalSeed = false);

    /// \brief Generate a random floating point number in the range [0,1).
    /// This arguably the most commonly used type of random number, and likely
    /// the function you are looking for.
    float randomFloatCO()const;

    /// \brief Generate a random floating point number in the range (0,1).
    float randomFloatOO()const;

    /**
     * Generate a random int in the range
     * [begin, end).
     */
    int randomIntCO(int begin, int end)const;

private:
    mutable tinymt32_t rng;

    //No copying!
    RNG& operator=(const RNG& other)const;
    RNG(const RNG& other);
};

inline RNG::RNG(unsigned int seedVal){
    tinymt32_init(&rng, seedVal);
}

inline RNG::RNG(bool useTemporalSeed){
    uint32_t seed_array[5] = { 4546546, 5265656, 1013915, 13, 94634 };
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
        tinymt32_init_by_array(&rng, seed_array, 1);
    }
}

inline float RNG::randomFloatCO()const{
#ifdef USE_UNION_TRICK
    return tinymt32_generate_float01(&rng);
#else
    return tinymt32_generate_float(&rng);
#endif
}
inline float RNG::randomFloatOO()const{
    return tinymt32_generate_floatOO(&rng);
}

//Note - This does not work!
//inline float RNG::randomFloatOC()const{
//    return tinymt32_generate_floatOC(&rng);
//}

inline int RNG::randomIntCO(int begin, int end)const{
    Assert(begin <= end);
    const uint32_t randomNum = tinymt32_generate_uint32(&rng);
    //absolute integer difference
    const uint32_t absDiff = ((end - begin) >= 0 ? (end - begin) : -(end - begin));
    const int offset = (int)(randomNum % absDiff);
    const int ret = begin + offset;
    Assert(ret >= begin && ret < end);
    return ret;
}


#endif //RNG_H
