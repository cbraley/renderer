#ifndef SHAPE_H
#define SHAPE_H

#include "math/Transform.h"
#include "materials/BRDF.h"
#include "materials/LambertianBRDF.h"
#include "geom/RaySurfIntersection.h"
#include "geom/BoundingBox.h"
#include "materials/BRDF.h"
class Light; //Forward declaration to avoid circular include

/**
 *  Interface for all classes that can be interested by rays, or be refined to become
 *  intersectable.
 */
class Shape{
protected:
    //Object-2-world and World-2-object transformation matrices
    Transform o2w, w2o;
    BRDF* brdf;
    Light* l;

public:

    /**
     *  Parameterized constructor.
     */
    Shape(const Transform& objToWold, Light* light = NULL) :
        o2w(objToWold), w2o(objToWold.inverse()), brdf(NULL),
        l(light)
    {
        //brdf = new LambertianBRDF();    
    }


    virtual ~Shape(){ delete brdf; brdf = NULL; }

    virtual bool intersectsDirectly()const = 0;
    virtual std::vector<Shape*> refine()const{
        Assert(false);
        std::vector<Shape*> s; return s;
    }

    virtual bool intersect(const Ray& r, RaySurfIntersection& res)const{
        Assert(false);
        return false;
    }


    //Get and set object material
    virtual BRDF* getBRDF()const{ return brdf; }
    virtual void setBRDF(BRDF* newMat){ delete brdf; brdf = newMat; }

    inline void setEmitter(Light* lnew){ l = lnew; }
    inline bool isEmitter()const{ return l != NULL; }
    inline Light* getEmitter()const{ return l; }

    /**
     *  Return bounding box in object space.
     */
    virtual BoundingBox objectSpaceBound()const = 0;

    /**
     *  Return bound in world space.  A default implementation exists,
     *  assuming one can bound the shape in object space.  However,
     *  for many shapes it is possible to compute a tighter bound.
     */
    virtual BoundingBox worldSpaceBound()const;

    virtual float surfaceArea()const = 0;

private:
    Shape();
    Shape(const Shape& other);
    Shape& operator=(const Shape& rhs);

};


#endif //SHAPE_H
