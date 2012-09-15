#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cassert>
#include <iostream>
//--
#include "utils/Assert.h"

namespace Interpolation{

// Let f be a function f(x):reals->reals
// Linearly interpolate as x goes from x_0 to x_1, return values
// from y_0 to y_1
//
// Note that the following conditions MUST hold:
//   I)  x_0 <= x <= x1
//   II)  x_0 != x_1
//
// If you are unsure that condition I will be true,
// try using the (slower) safeLerp.* function.
//
// TODO: Verify edge behavior
template<typename T>
T lerp(const T x, const T x_0, const T x_1, const T y_0, const T y_1){
    Assert(x_0 <= x && x <= x_1);
    Assert(x_0 != x_1); //If this vas true t would become NaN fro divide by 0.0

    const T t = (x - x_0)/(x_1 - x_0);
    const T ONE = static_cast<T>(1.0);
    // Note that this formulation is more numerically stable than
    //   y_0 + t(y_1 - y_0)
    // since in the above formulation the (y_1 - y_0) term can cause
    // catastrophic loss of precision.
    return y_0 * (ONE - t) + t * y_1;
}

// Same as the "lerp" function but x is allowed to be outside the
// range [x_0, x_1].  In the case that x is outside this range this function
// will always return 0.0.
template<typename T>
T safeLerpExtrapolateAlways0(const T x, const T x_0, const T x_1, const T y_0, const T y_1){
    const T ZERO = static_cast<T>(0.0);
    return (x < x_0 || x > x_1) ?
        ZERO : //If we are out of bounds return 0.0
        lerp<T>(x, x_0, x_1, y_0, y_1); //...otherwise just lerp
}

// Compute the area of a trapezoid. y_0 and y_1 may be neative.
//            /|  .
//           / |  .
//          /  |  .
//         /   |  .
//        /    |  y_1
//       /     |  .
//      /      |  .
//    . |      |  .
//  y_0 |      |  .
//    . |______|  .
//      delta_x
//
template<typename T>
T signedTrapezoidArea(const T delta_x, const T y_0, const T y_1){
    //TODO: What is the signed area???
    Assert(false);
    return 0.0;
}
}

#endif //INTERPOLATION_H
