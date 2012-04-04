#ifndef RNG_H
#define RNG_H

#include "tinymy32.h"

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
    RNG(unsigned int seedVal);
void tinymt32_init(tinymt32_t * random, uint32_t seed);
   
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


private:
    tinymy32_t rng;

    //No copying!
    RNG& operator=(const RNG& other)const;
    RNG(const RNG& other);
};


inline float randomFloatOO()const{
    return tinymt32_generate_floatOO(rng);
}
inline float randomFloatOC()const{
    return tinymt32_generate_floatOC(rng);
}

inline static float tinymt32_generate_floatOC(tinymt32_t * random) {
    tinymt32_next_state(random);
    return 1.0f - tinymt32_generate_float(random);
}

/**
 * This function outputs floating point number from internal state.
 * This function returns neither 0.0 nor 1.0.
 * @param random tinymt internal status
 * @return floating point number r (0.0 < r < 0.0)
 */
inline static float tinymt32_generate_floatOO(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper_conv_open(random) - 1.0f;
}



#endif //RNG_H
