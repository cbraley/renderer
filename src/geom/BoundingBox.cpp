#include "geom/BoundingBox.h"
//--
#include <cmath>
//--
#include "utils/Assert.h"

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


void BoundingBox::splitX(BoundingBox& ret1, BoundingBox& ret2, float splitDist)const{
    Assert(splitDist > 0.0f && splitDist < 1.0f);
    const float dx = fabs(maxPoint.x - minPoint.x);
    ret1.minPoint = minPoint;
    ret1.maxPoint = Point(
        minPoint.x + dx * splitDist,
        maxPoint.y,
        maxPoint.z );
    ret2.minPoint = ret1.maxPoint;
    ret2.maxPoint = maxPoint;
}

void BoundingBox::splitY(BoundingBox& ret1, BoundingBox& ret2, float splitDist)const{
    Assert(splitDist > 0.0f && splitDist < 1.0f);
    const float dy = fabs(maxPoint.y - minPoint.y);
    ret1.minPoint = minPoint;
    ret1.maxPoint = Point(
        maxPoint.x,
        minPoint.y + dy * splitDist,
        maxPoint.z );
    ret2.minPoint = ret1.maxPoint;
    ret2.maxPoint = maxPoint;
}
void BoundingBox::splitZ(BoundingBox& ret1, BoundingBox& ret2, float splitDist)const{
    Assert(splitDist > 0.0f && splitDist < 1.0f);
    const float dz = fabs(maxPoint.z - minPoint.z);
    ret1.minPoint = minPoint;
    ret1.maxPoint = Point(
        maxPoint.x,
        maxPoint.y,
        minPoint.z  + dz * splitDist);
    ret2.minPoint = ret1.maxPoint;
    ret2.maxPoint = maxPoint;
}


Point BoundingBox::centroid()const{
    const Point temp = (minPoint + maxPoint);
    return Point(0.5f * temp.x, 0.5f * temp.y, 0.5f * temp.z);
}

