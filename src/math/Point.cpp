#include "Point.h"
//--
#include <cmath>

//#define POINT_EXACT_COMPARE

static const float POINT_COMPARE_EPS = 0.00001f;

bool Point::operator==(const Point& other)const{
#ifdef POINT_EXACT_COMPARE
    return x == other.x && y == other.y && z == other.z;
#else
    const float xDiff = std::fabs(x - other.x);
    const float yDiff = std::fabs(y - other.y);
    const float zDiff = std::fabs(z - other.z);
    return xDiff < POINT_COMPARE_EPS && yDiff < POINT_COMPARE_EPS && zDiff < POINT_COMPARE_EPS;
#endif
}

bool Point::operator!=(const Point& other)const{
    return !(*this == other);
}

