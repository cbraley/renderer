#ifndef BVH_H
#define BVH_H

#include <iostream>
#include <stack>
#include <vector>
//--
#include "geom/BoundingBox.h"

/**
 *  Templated BVH class.
 *
 *
 */
template<typename T>
class BVH{
public:
    enum SplitAxis{AXIS_X, AXIS_Y, AXIS_Z};



private:

    typedef struct BVHNode{
        T** data;
        int numShapes;
        BoundingBox bbox;
        BVHNode* children[2];
        SplitAxis axis;
    }BVHNode;

};

#endif //BVH_H
