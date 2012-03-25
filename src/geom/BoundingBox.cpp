#include "BoundingBox.h"
//--
#include "Assert.h"

std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox){
    os << "AABB( min = " << bbox.minPoint <<
        ", max = " << bbox.maxPoint << ")";
    return os;
}

bool BoundingBox::operator==(const BoundingBox& rhs)const{
    return minPoint == rhs.minPoint && maxPoint == rhs.maxPoint;
}

bool BoundingBox::operator!=(const BoundingBox& rhs)const{
    return (this != &rhs) || (!(*this == rhs));
}

