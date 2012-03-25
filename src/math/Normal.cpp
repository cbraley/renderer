#include "Normal.h"

bool Normal::operator==(const Normal& other)const{
    return (this == &other) ||
        (x == other.x && y == other.y && z == other.z);
}

bool Normal::operator!=(const Normal& other)const{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Normal& vec){
    os << "normal<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
    return os;
}

std::istream& operator>>(std::istream& is, Normal& vec){
    return is >> vec.x >> vec.y >> vec.z;
}

