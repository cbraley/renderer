#include "primitives/Shape.h"

BoundingBox Shape::worldSpaceBound()const{
    return o2w(objectSpaceBound());
}
