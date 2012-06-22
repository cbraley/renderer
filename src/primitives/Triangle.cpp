#include "primitives/Triangle.h"
//--
#include "utils/Assert.h"
#include "utils/GeomUtils.h"

Triangle::Triangle(const Point& pt1, const Point& pt2, const Point& pt3) :
    Shape(Transform()), v0(pt1), v1(pt2), v2(pt3)
{
    //Make sure we don't have a degenerate triangle
    Assert(!GeomUtils::arePointsColinear(v0,v1,v2) );
}

bool Triangle::intersect(const Ray& ray, RaySurfIntersection& res)const{
    const bool didHit = triangleIntersectHelper(ray, v0, v1, v2, res);
    res.shp = this;
    return didHit;
}

BoundingBox Triangle::objectSpaceBound()const{
    Assert(false);
    //TODO: Triangles store only 3 vertices in world space.
    //the architecture should be changed so that there is a base
    //abstract "Shape" class
    //which has a subclass:
    //TransformedShape
    //Triangles should inherit from Shape directly, whereas Spheres and other things should
    //inherit from Transformed shape
    return BoundingBox();
}

BoundingBox Triangle::worldSpaceBound()const{
    return BoundingBox(v0,v1).Union(v2);
}


float Triangle::surfaceArea()const{
    Vector edge1 = v1 - v0;
    Vector edge2 = v2 - v0;
    return 0.5f * (edge1.cross(edge2)).magnitude();
}



