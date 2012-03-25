#ifndef CONSTANTS_H
#define CONSTANTS_H

//_WIN32 is defined in BOTH 64 and 32 bit windows
//Here I turn the frustrating min and max windows macros off...
#ifdef _WIN32
    #define NOMINMAX
    #include <windows>

    //On Windows, we need to define _USE_MATH_DEFINES to enable M_PI
    #define _USE_MATH_DEFINES
    #include <cmath>
#else
    #include <cmath> //On *nix, M_PI is defined by default
#endif

#include <limits>

//Min and max values of primitive types----------------------------------------
const float MAX_FLOAT_VAL = std::numeric_limits<float>::max();
const float MIN_FLOAT_VAL = std::numeric_limits<float>::min();

const float MAX_INT_VAL = std::numeric_limits<int>::max();
const float MIN_INT_VAL = std::numeric_limits<int>::min();

//Min positve values of numeric types------------------------------------------
const float MIN_POS_FLOAT  = std::numeric_limits<float>::epsilon();
const float MIN_POS_DOUBLE = std::numeric_limits<double>::epsilon();


//Misc mathematical constants--------------------------------------------------
const float PI_FLOAT         = M_PI;
const float INVERSE_PI_FLOAT = (0.5f*M_PI);
const float TWO_PI_FLOAT     = (2.0f*M_PI);
const float PI_OVER_2_FLOAT  = (M_PI*0.5f);
const float FOUR_PI_FLOAT    = (4.0f*M_PI);
const float PI_DOUBLE        = M_PI;

#endif //CONSTANTS_H

