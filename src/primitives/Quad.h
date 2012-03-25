#ifndef QUAD_H
#define QUAD_H

#include "RaySurfIntersection.h"
#include "Shape.h"
#include "Point.h"
#include "Vector.h"

class Quad : public Shape{
public:
    Quad();

    /**
     *  Construct a quadrilateral from 4 points. Specify points in a counter 
     *  clockwise order.  Points should always be coplanar.  If they are not,
     *  an assertion will fail in debug mode.  However, in release mode there
     *  will be no error message printed.
     */
    Quad(const Point& p0, const Point& p1, const Point& p2, const Point& p3,
        Light* light = NULL);

    virtual ~Quad(){}

    virtual bool intersect(const Ray& r, RaySurfIntersection& res)const;
    virtual bool intersectsDirectly()const{return true;}

    virtual BoundingBox objectSpaceBound()const;
    virtual BoundingBox worldSpaceBound()const;
    
    virtual float surfaceArea()const;

    Point randomPt()const; //TODO: Remove this once real sampling is implemented

private:
    Point v_00, v_10, v_11, v_01; //CCW
};

#endif //QUAD_H
