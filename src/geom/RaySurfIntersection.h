#ifndef RAY_SURF_INTERSECTION_H
#define RAY_SURF_INTERSECTION_H

#include "geom/Ray.h"
#include "math/Vector.h"
#include "math/Normal.h"
#include "math/Point.h"
#include "utils/Assert.h"
class Shape;


/**
 *  Simple class representing the intersection of a geometric Ray
 *  with a Shape in the scene.
 */
class RaySurfIntersection{
public:
    Ray ray;          //Ray that hit the surface
    float tHit;       //parameter value at which intersection occured along ray
    Point locWS;      //Hit location in world space
    Vector n;         //Normal vector at hit location
    Vector t1, t2;    //Tangent vectors at intersection
    const Shape* shp; //Pointer to intersected geometry

    bool err;         //Was there an error?
    //TODO: Remove "err" this is for debugging
};

#endif //RAY_SURF_INTERSECTION_H
