#include "math/IrregularlySampledFunction.h"

#include <algorithm>
#include "utils/Assert.h"
#include <utility>

IrregularlySampledFunction::IrregularlySampledFunction(){}

//TODO: Pass const refs
static bool compareByXs(struct std::pair<float,float> a, struct std::pair<float,float> b){
    return a.first < b.first;
}
static bool equalityByXs(struct std::pair<float,float> a, struct std::pair<float,float> b){
    return a.first == b.first;
}

IrregularlySampledFunction::IrregularlySampledFunction(const std::vector<float>& unsorted_xs,
    const std::vector<float>& ys){
    
    Assert(unsorted_xs.size() == ys.size());

    //Make a list of pairs of xs and ys
    std::vector< std::pair<float,float> > pairList;
    pairList.reserve(unsorted_xs.size());
    for(size_t i = 0; i < unsorted_xs.size(); ++i){
        const float x = unsorted_xs[i];
        const float y = ys[i];
        std::pair<float,float> currPair = std::make_pair(x,y);
        pairList.push_back(currPair);
    }

    //Sort the list of pairs by x
    std::sort(pairList.begin(), pairList.end(), compareByXs);

    //What to do in the presence of duplicate x's?
    std::vector< std::pair<float,float> >::iterator adjacentEqualElems = 
        std::adjacent_find(pairList.begin(), pairList.end(), equalityByXs);
    const bool hasDuplicateXs = adjacentEqualElems != pairList.end();
    Assert(! hasDuplicateXs);

    //Place the elements into the array
    xs.reserve(pairList.size());
    fs.reserve(pairList.size());
    for(size_t i = 0; i < pairList.size(); ++i){
        xs.push_back(pairList[i].first);
        fs.push_back(pairList[i].second);
    }
}


bool IrregularlySampledFunction::addSample(float x, float y){
    Assert(invariantHolds());

    // Get an iterator to the first element x' s.t
    // x' >= x
    std::vector<float>::iterator insertLocXs = 
        std::lower_bound(xs.begin(), xs.end(), x);

    // Check if this is a duplicate element
    const bool attemptToInsertDuplicateX = 
        insertLocXs != xs.end() && //Boolean short circuiting saves us from executing
        *insertLocXs == x; //the *insertLocXs if the iterator is at the end of the array
    if(attemptToInsertDuplicateX){
        return false;
    }

    //Now we need to add in *both* the x and y elements!
    //This necessitates resizing the array, which is slow...
    const size_t xIndex = insertLocXs - xs.begin();
    xs.insert(insertLocXs, x);
    fs.insert(fs.begin() + xIndex, y);

    //Return true
    Assert(invariantHolds());
    return true;
}

IrregularlySampledFunction::~IrregularlySampledFunction(){}
 

bool IrregularlySampledFunction::invariantHolds()const{
    if(xs.size() != fs.size()){ //Sizes should match
        return false;
    }else{ //Check for sorted-ness
        for(size_t i = 1; i < xs.size(); ++i){
            if(xs[i] < xs[i-1]){
                return false;
            }
        }
    }
    return true;
}


std::ostream& operator<<(std::ostream& os, const IrregularlySampledFunction& func){
    for(size_t i = 0; i < func.xs.size(); ++i){
        os << "f("<< func.xs[i] << ") = " << func.fs[i] << "   ";
    }
    return os;
}


float IrregularlySampledFunction::defIntegral(float x_begin, float x_end)const{
    // If limits are improper, just return 0.0f
    if(x_end < x_begin){ return 0.0f; } //TODO: is this a good strategy for catching errors?

    //First, limit our integration to the area over which our function is defined
    const float minXVal = xs[0];
    const float maxXVal = xs[xs.size()-1];
    x_begin = std::max<float>(x_begin, minXVal);
    x_end   = std::min<float>(x_end  , maxXVal);

    //Find the indices across which we need to consider during out integration
    std::vector<float>::const_iterator elemAtOrAfterEnd = std::lower_bound(xs.begin(), xs.end(), x_end  );
    std::vector<float>::const_iterator elemAfterBegin   = std::upper_bound(xs.begin(), xs.end(), x_begin);
    const size_t startIdx = (elemAfterBegin - xs.begin())-1;
    const size_t endIdx   = (elemAtOrAfterEnd - xs.begin());

    //The first and last trapezoid must be handled separately, since only a portion of their 
    //area contributes to the sum
    const float fBegin = Interpolation::lerp<float>(x_begin, xs[startIdx], xs[startIdx+1], fs[startIdx], fs[startIdx+1]);
    const float fEnd   = Interpolation::lerp<float>(x_end  , xs[endIdx-1], xs[endIdx    ], fs[endIdx-1], fs[endIdx    ]);
    const float firstTrapArea = Interpolation::signedTrapezoidArea(xs[startIdx+1] - x_begin, fBegin, fs[startIdx+1]);
    const float lastTrapArea  = Interpolation::signedTrapezoidArea(x_end - xs[endIdx-1]    , fEnd  , fs[endIdx-1  ]);

    return firstTrapArea + lastTrapArea + defIntegralHelper(startIdx, endIdx);
}


float IrregularlySampledFunction::defIntegralHelper(size_t startInclusive, size_t endInclusive)const{
    return -999.0f; //TODO
}

float IrregularlySampledFunction::defIntegral()const{
    return defIntegralHelper(0, xs.size()-1);
}


