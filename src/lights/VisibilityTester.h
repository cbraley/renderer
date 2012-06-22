#ifndef VISIBILITY_TESTER_H
#define VISIBILITY_TESTER_H

#include "scene/Scene.h"
#include "accel/AccelStructure.h"
#include "primitives/Shape.h"

/**
 *  The VisibilityTester class takes on the role of "shadow rays" in a RayTracer.
 *  The VisibilityTester class is used to make sure that we only shoot shadow rays
 *  when absolutely necessary.
 */
class VisibilityTester{
public:
    float minT, maxT;     //Extent of ray to check along
    const Shape* lg;      //Light source geometry (should NULL for point lights)
    Ray r;                //Ray to potentially trace
    const AccelStructure* geom; //Scene acceleration structure

    /**
     *  Constructor.
     *  @param surfacePt is the location in world space that you wish to check if a light sample can "see."
     *  @param lightPoint is the location in world space on the light.
     *  @param lightGeom is a pointer to the lights geometry(a Shape).  This may be NULL for sources like point lights.
     *  @param sceneGeom is the scene acceleration structure.
     *  @param epsilonVal is the value by which to move the origin of the shadow ray forward, and the parameter
     *   value which the light is located at backward.  This can be used to avoid the self-intersection problem
     *   so common in raytracing.  Defaults to 0.0f.
     */
    VisibilityTester(const Point& surfacePt, const Point& lightPoint, const Shape* lightGeom,
        const AccelStructure* sceneGeom, float epsilonVal = 0.0f) :
        minT(0.0f), maxT(1.0f), lg(lightGeom), r(surfacePt, (lightPoint - surfacePt)), geom(sceneGeom)
    {
        Assert(geom != NULL);
        maxT = (surfacePt - lightPoint).magnitude();
        if(epsilonVal != 0.0f){
            minT += epsilonVal;
            maxT -= epsilonVal;
        }
        Assert(maxT > minT);
    }

    /**
     *  Check if the light sample is visible.
     *  @eturn true if the light is visible, false otherwise.
     */
    bool visible(const Shape* sPtr = NULL)const;
};

//Inline function definitions -------------------------------------------------

inline bool VisibilityTester::visible(const Shape* sPtr)const{
    return !(geom->doesIntersect(r, minT, maxT, sPtr == NULL ? lg : sPtr));
}

#endif //VISIBILITY_TESTER_H
