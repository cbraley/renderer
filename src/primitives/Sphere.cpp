#include "primitives/Sphere.h"

#include "math/MathUtils.h"
#include "utils/Assert.h"
#include "math/Transform.h"
#include "utils/Constants.h"


/// \brief Default c-tor creates a sphere with radius 1 at the orogin.
Sphere::Sphere() :
    Shape(Transform()), r(1.0f)
{}

Sphere::Sphere(const Transform& objToWold,
    const float radius) :
    Shape(objToWold), r(radius)
{
    Assert(r > 0.0f);
}

bool Sphere::intersect(const Ray& ray, RaySurfIntersection& res)const{
    res.shp = NULL;

    //Transform ray to object space
    const Ray rOb = w2o(ray);

    const Vector o = Vector(rOb.getOrigin().x, rOb.getOrigin().y, rOb.getOrigin().z);
    const Vector d = rOb.getDir();
    const float A = d.dot(d);
    const float B = 2.0f * (d.dot(o));
    const float C = o.dot(o) - (r * r);


    struct MathUtils::QuadraticEqnRes<float> slv =
        MathUtils::solveQuadratic<float>(A, B, C);
    float tHitFinal = 0.0f; //After the below code this will eventually be set to
    //the first hit point in front of the camera
    if(slv.solCount == 0){
        return false;
    }else if(slv.solCount == 1){
        tHitFinal = slv.sol1;
        if(tHitFinal < 0.0f){
            //No solutions in front of camera
            return false;
        }
    }else{ //2 solutions
        //Find smallest t value that is > 0.0f
        if(slv.sol1 < 0.0f && slv.sol2 < 0.0f){
            //No solutions in front of camera
            return false;
        }else{
            //At least one hit in front of camera
            slv.sol1 = slv.sol1 < 0.0f ? MAX_FLOAT_VAL : slv.sol1;
            slv.sol2 = slv.sol2 < 0.0f ? MAX_FLOAT_VAL : slv.sol2;
            tHitFinal = std::min<float>(slv.sol1, slv.sol2);
        }
    }

    //Make sure hit is in front of camera
    Assert(tHitFinal >= 0.0f);

    res.tHit = tHitFinal;
    res.locWS = ray(res.tHit);
    Vector normalVecAtHit = res.locWS - o2w(Point(0.0f,0.0f,0.0f));
    res.n = normalVecAtHit.getNormalized();
    res.shp = this;
    return true;
}


BoundingBox Sphere::objectSpaceBound()const{
    return BoundingBox(
        Point(-r,-r,-r), //Lower left corner
        Point( r, r, r)  //Upper right corner
    );
}

float Sphere::surfaceArea()const{
    return 4.0f * PI_FLOAT * r * r;
}



