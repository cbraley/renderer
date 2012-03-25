#include "Shape.h"

BoundingBox Shape::worldSpaceBound()const{
    return o2w(objectSpaceBound());
}
