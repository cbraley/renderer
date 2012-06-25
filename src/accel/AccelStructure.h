#ifndef ACCEL_STRUCTURE_H
#define ACCEL_STRUCTURE_H

#include <vector>
//--
#include "geom/RaySurfIntersection.h"
#include "geom/Ray.h"
#include "primitives/Shape.h"
#include "utils/Constants.h"

/**
 *  Base class for ray-tracing acceleration structures (BVH, KD-tree, etc).
 */
class AccelStructure{
public:

    /**
     *  Build the scene acceleration structure.  Should always be called before
     *  any rays are traced.
     */
    virtual void buildAccelStructure(const std::vector<Shape*>& sceneData) = 0;

    /**
     *  Trace a single ray using the acceleration strucure to potentially
     *  speed up the process.
     */
    virtual RaySurfIntersection getClosestIntersection(const Ray& r, bool& hitAnything)const = 0;

    /**
     *  Check if the given ray intersects ANYTHING along ray r, betweeen the points
     *  r(minDist) and r(maxDist).
     *
     *  @param minDist The t parameter of the ray must be greater than minDist
     *   when an intersection occurs in order for the intersection to count.
     *   Defaults to 0.0f.

     *  @param maxDist The t parameter of the ray must be less than maxDist
     *   when an intersection occurs in order for the intersection to count.
     *   Defaults to the maximum possible float value.
     *
     *  @ignoreShape is a pointer to a single shape who should be ignored
     *   and not counted as an intersection.  Defaults to NULL, which indicates
     *   that all shapes should be counted.
     */
    virtual bool doesIntersect(const Ray& r,
        float minDist = 0.0f, float maxDist = Constants::MAX_FLOAT_VAL,
        const Shape* ignoreShape = NULL)const = 0;


    //TODO: Consider an iterator for the below situation.
    //Do acceleration structures exist where a random access
    //requirement is unattainable/a bad idea?

    /**
     *  Return the number of shapes in the scene.
     *  @return the number of shapes.  Arguments to getShape(...)
     *    must be integer-valued and in the range [0, getNumShapes())
     */
    virtual size_t getNumShapes()const = 0;

    /**
     *  Random access to a individual shapes in the scene.
     *  See getNumShapes().
     */
    virtual const Shape* getShape(size_t idx)const = 0;

};

#endif //ACCEL_STRUCTURE_H
