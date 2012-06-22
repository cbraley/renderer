#include "math/Vector.h"

static const float VECTOR_COMPARE_EPS = 0.00001f;

std::ostream& operator<<(std::ostream& os, const Vector& vec){
    os << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
    return os;
}
std::istream& operator>>(std::istream& is, Vector& vec){
    return is >> vec.x >> vec.y >> vec.z;
}

bool Vector::operator==(const Vector& other)const{
    const float xDiff = std::fabs(x - other.x);
    const float yDiff = std::fabs(y - other.y);
    const float zDiff = std::fabs(z - other.z);
    return xDiff < VECTOR_COMPARE_EPS && yDiff < VECTOR_COMPARE_EPS && zDiff < VECTOR_COMPARE_EPS;
}

bool Vector::operator!=(const Vector& b)const{ return !(*this == b); }


bool Vector::isNormalized()const{
    return getNormalized() == (*this);
}

