#ifndef PRIMITIVE_TRIANGLE_H
#define PRIMITIVE_TRIANGLE_H

#include "RaySurfIntersection.h"
#include "Shape.h"
#include "Vector.h"
#include "Point.h"

//TODO: Triangle class eventually needs a large overhaul for 2 reasons
//    1 - Triangle's are typically in scenes in a mesh.  Each triangle storring
//    the world space location of each of its vertices is wasteful!
//    we should instead load all the verts into an array in memory, and then
//    have each triangle store 3 indices into the array.  This will save mem.
//
//    2 - Triangles inherit from shape, which means that they have 2 matrices for
//    transforming to/from world/object space.  This wastes memory, since triangles
//    are not implemented this way.  We should instead make a base class further up
//    the heirarchy that precedes Shape.
//

class Triangle : public Shape{
public:
    Triangle(const Point& pt1 = Point(-1.0f, 0.0f, 2.0f),
        const Point& pt2      = Point( 1.0f, 0.0f, 2.0f),
        const Point& pt3      = Point( 0.0f, 1.0f, 2.0f)
    );

    virtual ~Triangle(){}

    virtual bool intersect(const Ray& ray, RaySurfIntersection& res)const;
    virtual bool intersectsDirectly()const{ return true; }

    virtual BoundingBox objectSpaceBound()const;
    virtual BoundingBox worldSpaceBound()const;

    virtual float surfaceArea()const;

private:
    Point v0,v1,v2;
    //TODO: Remove this!  See reason 1 above!
};

//Inline function definitions -------------------------------------------------

inline bool triangleIntersectHelper(const Ray& ray, const Point& v0, const Point& v1, const Point& v2,
    RaySurfIntersection& res){

    //Code adapted from:
    //"Fast Minimum Storage Ray Triangle Intersection Test"
    //(Moller-Trombore test)
    const float TRI_EPSILON = 0.000001f;
    res.shp = NULL;

    Vector edge1 = v1 - v0;
    Vector edge2 = v2 - v0;
    Vector pvec = ray.getDir().cross(edge2);

    float det = edge1.dot(pvec);
    if(det > -TRI_EPSILON && det < TRI_EPSILON){
        //No hit
        return false;
    }else{
        //Hit the plane
        float inv_det = 1.0f / det;
        Vector tvec = ray.getOrigin() - v0;

        //Test U
        float u = tvec.dot(pvec) * inv_det;
        if(u < 0.0f || u > 1.0f){
            return false;
        }

        //Test V
        Vector qvec = tvec.cross(edge1);
        float v = ray.getDir().dot(qvec) * inv_det;
        if(v < 0.0f || u + v > 1.0f){
            return false;
        }

        //Must be a hit, calculate distance to hit
        float t = edge2.dot(qvec) * inv_det;

        //Make sure hit is not behind camera
        if(t <= 0.0f){
            return false;
        }


        //Set the output params
        res.tHit = t;
        res.locWS = ray(t);
        res.n = edge1.cross(edge2).getNormalized();

        return true;
    }


}

#endif //PRIMITIVE_TRIANGLE_H
