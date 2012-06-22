#ifndef PRIMITIVE_SPHERE_H
#define PRIMITIVE_SPHERE_H

#include "geom/RaySurfIntersection.h"
#include "primitives/Shape.h"

class Sphere : public Shape{
private:
    float r; //Radius
    //Position is implicit in the w2o in Shape class

public:

    /// \brief Default c-tor creates a sphere with radius 1 at the orogin.
    Sphere();

    virtual ~Sphere(){}

    Sphere(const Transform& objToWold, const float radius = 1.0f);

    virtual bool intersect(const Ray& ray, RaySurfIntersection& res)const;

    virtual bool intersectsDirectly()const{ return true; }

    inline float getRadius()const{ return r; }

    virtual BoundingBox objectSpaceBound()const;

    virtual float surfaceArea()const;

};

#endif //PRIMITIVE_SPHERE_H
