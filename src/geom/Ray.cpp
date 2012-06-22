#include "geom/Ray.h"

//Note that the below definitions of the == and != operators depend
//on how == and != are implemented for Vector and Point
bool Ray::operator==(const Ray& other)const{
    return (this == &other) || (dir == other.dir && o == other.o);
}
bool Ray::operator!=(const Ray& other)const{
    return !(*this == other);
}


std::ostream& operator<<(std::ostream& os, const Ray& r){
    os << r.o << " + " << r.dir << "t";
    return os;
}

