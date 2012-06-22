#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "utils/Assert.h"

namespace Interpolate{

    inline float linear(float v, float A, float B){
        return v * (B-A) + A;
    }






}

#endif //INTERPOLATE_H
