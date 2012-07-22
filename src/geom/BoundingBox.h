#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <string>
#include <iostream>
#include <cmath>
//--
#include "math/Vector.h"
#include "utils/Constants.h"
#include "math/Point.h"


/**
 * Axis-aligned bounding box class. (AABB)
 *
 * Client code is allowed to modify the publically accessable
 * minPoint and maxPoint variables.
 *
 * Note that some functions are implemented both as member functions as well 
 * as static member functions.  The static member functions are there to make 
 * allow STL algorithms that require function pointers(ex std::accumulate) usable without
 *     -Writing 2 line boilerplate functions each time we want to use std::accumulate
 *     -Using lambdas(I am not currently using C++0x so no lambdas)
 *     -boost::bind (I am not using boost)
 */
class BoundingBox{
public:
    ///minPoint and maxPoint specify the extent of the box
    ///  minPoint has the smallest x,y and z values of any box vertex
    ///  maxPoint has the largest x,y and z values of any box vertex
    ///
    ///These are publicly accessable to the client code
    Point minPoint, maxPoint;

    /**
     *  Defaults to a DEGENERATE bounding box,
     *  ensuring that this box essentially takes up
     *  no area and future union operations will work.
     */
    BoundingBox();

    /**
     *   Copy constructor.
     */
    BoundingBox(const BoundingBox& other);

    /**
     *  Assignment operator.
     */
    BoundingBox& operator=(const BoundingBox& rhs);

    /**
     *  Find the bounding box around two points.
     *  The order of the points passed in does not
     *  matter.
     */
    BoundingBox(const Point& p1, const Point& p2);

    /**
     *  Compute the union of the *this bounding box
     *  with a  given point.
     *  The method name is capitalized because "union" is a reserved word in C/C++.
     *  Returns a reference to *this.
     */
    BoundingBox& Union(const Point& newPt);


    /**
     *  Compute the union of the *this bounding box
     *  with another bounding box.
     *  The method name is capitalized because "union" is a reserved word in C/C++.
     *  Returns a reference to *this.
     */
    BoundingBox& Union(const BoundingBox& other);
    
    /**
     *  Volume of box.
     */
    float getVolume()const;

    /**
     *  Surface area of box.
     */
    float getSurfaceArea()const;

    /**
     *  Grow or shrink the bounding box by delta.
     *  delta can be negative to contract the box.
     *
     *  The boxs corners will each move away from the opposing
     *  corner by delta.
     *  @param delta is the displacement amount
     */
    void expandBy(float delta);
    /**
     *  Get all 8 corners of a bounding box and place them
     *  into the 8 element array of Point's corners.
     *  @param corners is the array to update.
     */
    void getCorners(Point corners[8])const;

    void splitX(BoundingBox& ret1, BoundingBox& ret2, float splitDist = 0.5f)const;
    void splitY(BoundingBox& ret1, BoundingBox& ret2, float splitDist = 0.5f)const;
    void splitZ(BoundingBox& ret1, BoundingBox& ret2, float splitDist = 0.5f)const;


    /// \brief Compute cetroid(aka center of mass) of the bounding box.
    Point centroid()const;

    /**
     *  Overloaded stream insertion operator.  Prints a bounding box.
     *  @param os is the stream to print on
     *  @parm bbox is the box to print
     *  @return a reference to os
     */
    friend std::ostream& operator<<(std::ostream& os, const BoundingBox& bbox);

    bool operator==(const BoundingBox& rhs)const;
    bool operator!=(const BoundingBox& rhs)const;
};

//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------

inline BoundingBox::BoundingBox() :
    minPoint( Constants::MAX_FLOAT_VAL,  Constants::MAX_FLOAT_VAL,  Constants::MAX_FLOAT_VAL),
    maxPoint(-Constants::MIN_FLOAT_VAL, -Constants::MIN_FLOAT_VAL, -Constants::MIN_FLOAT_VAL)
{}

inline BoundingBox::BoundingBox(const BoundingBox& other) :
    minPoint(other.minPoint), maxPoint(other.maxPoint)
{}


inline BoundingBox& BoundingBox::operator=(const BoundingBox& rhs){
    minPoint = rhs.minPoint;
    maxPoint = rhs.maxPoint;
    return *this;
}

inline BoundingBox::BoundingBox(const Point& p1, const Point& p2){
    minPoint = Point( std::min(p1.x, p2.x), std::min(p1.y, p2.y),
        std::min(p1.z, p2.z) );
    maxPoint = Point( std::max(p1.x, p2.x), std::max(p1.y, p2.y),
        std::max(p1.z, p2.z) );
}

inline BoundingBox& BoundingBox::Union(const Point& newPt){
    minPoint.x = std::min(minPoint.x, newPt.x);
    minPoint.y = std::min(minPoint.y, newPt.y);
    minPoint.z = std::min(minPoint.z, newPt.z);

    maxPoint.x = std::max(maxPoint.x, newPt.x);
    maxPoint.y = std::max(maxPoint.y, newPt.y);
    maxPoint.z = std::max(maxPoint.z, newPt.z);

    return *this;
}

inline BoundingBox& BoundingBox::Union(const BoundingBox& other){
    minPoint.x = std::min(minPoint.x, other.minPoint.x);
    minPoint.y = std::min(minPoint.y, other.minPoint.y);
    minPoint.z = std::min(minPoint.z, other.minPoint.z);

    maxPoint.x = std::max(maxPoint.x, other.maxPoint.x);
    maxPoint.y = std::max(maxPoint.y, other.maxPoint.y);
    maxPoint.z = std::max(maxPoint.z, other.maxPoint.z);

    return *this;
}

inline float BoundingBox::getVolume()const{
    float xExtent = maxPoint.x - minPoint.x;
    float yExtent = maxPoint.y - minPoint.y;
    float zExtent = maxPoint.z - minPoint.z;
    return xExtent * yExtent * zExtent;
}

inline float BoundingBox::getSurfaceArea()const{
    float xExtent = maxPoint.x - minPoint.x;
    float yExtent = maxPoint.y - minPoint.y;
    float zExtent = maxPoint.z - minPoint.z;
    float a = (xExtent * yExtent);
    float b = (xExtent * zExtent);
    float c = (zExtent * yExtent);
    return 2.0f * (a + b + c);
}

inline void BoundingBox::expandBy(float delta){
    maxPoint += Vector(delta, delta, delta);
    minPoint -= Vector(delta, delta, delta);
}

inline void BoundingBox::getCorners(Point corners[8])const{
    corners[0] = Point(minPoint.x, minPoint.y, minPoint.z);
    corners[1] = Point(minPoint.x, maxPoint.y, maxPoint.z);
    corners[2] = Point(minPoint.x, minPoint.y, maxPoint.z);
    corners[3] = Point(minPoint.x, maxPoint.y, minPoint.z);
    corners[4] = Point(maxPoint.x, minPoint.y, minPoint.z);
    corners[5] = Point(maxPoint.x, maxPoint.y, maxPoint.z);
    corners[6] = Point(maxPoint.x, minPoint.y, maxPoint.z);
    corners[7] = Point(maxPoint.x, maxPoint.y, minPoint.z);
}



#endif //BOUNDING_BOX_H
