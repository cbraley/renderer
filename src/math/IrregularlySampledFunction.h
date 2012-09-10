#ifndef IRREGULARLY_SAMPLED_FUNCTION_H
#define IRREGULARLY_SAMPLED_FUNCTION_H

#include <vector>
#include "utils/Assert.h"
#include <algorithm>
#include <ostream>
//--
#include "math/Interpolation.h"

// Function f:Reals->Reals
// Point sampled at a potentially irregular intervals
// Values in between samples are looked up using linear interpolation,
// values outside the range are assumed to be 0.
// Supports definite integrals.
class IrregularlySampledFunction{
public:
    // Creation
    IrregularlySampledFunction();
    IrregularlySampledFunction(const std::vector<float>& unsorted_xs,
        const std::vector<float>& ys);

    // Destruction
    virtual ~IrregularlySampledFunction();

    // Getters
    float operator()(float x)const;
    float operator[](size_t xIdx)const;

    size_t getNumSamples()const;

    float defIntegral(float x_begin, float x_end)const;
    float defIntegral()const; //Integrate the whole function

    //Setters

    // Add a sample o the list.  In the event that there is already a sample
    // with the same x value, the same will not be added.  Returns true if the sample
    // was successfully added.
    bool addSample(float x, float y);

    friend std::ostream& operator<<(std::ostream& os, const IrregularlySampledFunction& func);

protected:
    bool invariantHolds()const;

    // Function is f(x)
    std::vector<float> xs; // x values of point samples (sorted).
    std::vector<float> fs; // function values per point.
    // so f(x[i]) = f[i]

    float defIntegralHelper(size_t startInclusive, size_t endInclusive)const;
};

inline float IrregularlySampledFunction::operator[](size_t xIdx)const{
    Assert(xIdx >= 0 && xIdx < xs.size());
    return fs[xIdx];
}

inline float IrregularlySampledFunction::operator()(float x)const{

    //If we are out of bounds, always return 0.0
    if(x < xs[0] || x > xs[xs.size()-1]){
        return 0.0f;
    }

    // Get an iterator to the first element x' s.t x' >= x
    std::vector<float>::const_iterator elem_at_or_after =
        std::lower_bound(xs.begin(), xs.end(), x);
    // Compute the index of x' (constant time)
    const size_t indexAtOrAfter = elem_at_or_after - xs.begin();
    // Linearly interpolate
    const size_t indexBefore = indexAtOrAfter - 1;
    //Note that we test indexAtOrAfter != 0, as opposed to indexBefore == -1,
    //since indexBefore may have wrapped around back to UINT_MAX if size_t is unsigned!!!
    return (indexAtOrAfter != 0) ?
        Interpolation::lerp<float>(x, xs[indexBefore], xs[indexAtOrAfter], fs[indexBefore], fs[indexAtOrAfter]) :
        fs[0]; //In this case the user specified they wanted to interpolate *exactly* xs[0]
}


inline size_t IrregularlySampledFunction::getNumSamples()const{
    Assert(xs.size() == fs.size());
    return xs.size();
}


#endif //IRREGULARLY_SAMPLED_FUNCTION_H


