#include "Quad.h"
//--
#include <cmath>
#include <iostream>
//--
#include "Assert.h"
#include "GeomUtils.h"
#include "Triangle.h"
#include "RNG.h"

//Intersection epsilon
static const float QUAD_EPSILON = 10e-6;

//Epsilon when making sure the points are all coplanar
static const float QUAD_COPLANAR_EPISLON = 1e-5;

Quad::Quad() : Shape(Transform(), NULL) {}

Quad::Quad(const Point& p0, const Point& p1, const Point& p2, const Point& p3, Light* light) :
    Shape(Transform(), light),
    v_00(p0), v_10(p1), v_11(p2), v_01(p3)
{
    //Check to make sure the 4 points are coplanar
    const Vector a = p1 - p0;
    const Vector b = p2 - p0;
    const Vector c = p2 - p1;
    const float absSpp = std::abs<float>(GeomUtils::scalarTripleProduct(a,b,c));
    Assert(absSpp < QUAD_COPLANAR_EPISLON);
}

bool Quad::intersect(const Ray& r, RaySurfIntersection& res)const{

    bool hitA = triangleIntersectHelper(r, v_00, v_01, v_10, res);
    if(hitA){
        res.shp = this;
        res.n = -res.n;
        return true;
    }
    bool hitB = triangleIntersectHelper(r, v_01, v_11, v_10, res);
    res.shp = this;
    res.n = -res.n;
    return hitB;

    //Code taken from:
    //http://jgt.akpeters.com/papers/LagaeDutre05/erqit.cpp.html
    //appears to have problems, unsure as to why it does not work properly.
    /*
    res.in = r;
    res.tHit = -1.0f;
    res.shp = NULL;
    res.err = false;

    //Start of code adapted from this source:
    float u,v,t;
    u = v = t = 0.0f;

    // Rejects rays that are parallel to Q, and rays that intersect the plane of
    // Q either on the left of the line V00V01 or on the right of the line V00V10.

    Vector E_01 = v_10 - v_00;
    Vector E_03 = v_01 - v_00;
    Vector P = r.getDir().cross(E_03);
    float det = E_01.dot(P);
    if(std::abs<float>(det) < QUAD_EPSILON){
        return false;
    }

    float inv_det = 1.0f / det;
    Vector T = r.getOrigin() - v_00;
    float alpha = T.dot(P) * inv_det;
    if(alpha < 0.0f){
        return false;
    }
   //if (alpha > float(1.0)) return false; // Uncomment if VR is used.
    Vector Q = T.cross(E_01);
    float beta = r.getDir().dot(Q) * inv_det;
    if(beta < 0.0f){
        return false;
    }
    //if (beta > float(1.0)) return false; // Uncomment if VR is used.

    if((alpha + beta) > 1.0f){
        // Rejects rays that intersect the plane of Q either on the
        // left of the line V11V10 or on the right of the line V11V01.

        Vector E_23 = v_01 - v_11;
        Vector E_21 = v_10 - v_11;
        Vector P_prime = r.getDir().cross(E_21);
        float det_prime = E_23.dot(P_prime);
        if(std::abs<float>(det_prime) < QUAD_EPSILON){
            return false;
        }
        float inv_det_prime = 1.0f / det_prime;
        Vector T_prime = r.getOrigin() - v_11;
        float alpha_prime = T_prime.dot(P_prime) * inv_det_prime;
        if(alpha_prime < 0.0f){
            return false;
        }
        Vector Q_prime = T_prime.dot(E_23);
        float beta_prime = r.getDir().dot(Q_prime) * inv_det_prime;
        if(beta_prime < 0.0f){
            return false;
        }
    }

    // Compute the ray parameter of the intersection point, and
    // reject the ray if it does not hit Q.
    t = E_03.dot(Q) * inv_det;
    if (t <= 0.0f){
        return false;
    }

    // Compute the barycentric coordinates of the fourth vertex.
    // These do not depend on the ray, and can be precomputed
    // and stored with the quadrilateral.
    float alpha_11, beta_11;
    Vector E_02 = v_11 - v_00;
    Vector n = E_01.cross(E_03);

    if((std::abs<float>(n.x) >= std::abs<float>(n.y))
        && (std::abs<float>(n.x) >= std::abs<float>(n.z)))
    {

        alpha_11 = ((E_02.y * E_03.z) - (E_02.z * E_03.y)) / n.x;
        beta_11  = ((E_01.y * E_02.z) - (E_01.z * E_02.y)) / n.x;

    }else if((std::abs<float>(n.y) >= std::abs<float>(n.x))
        && (std::abs<float>(n.y) >= std::abs<float>(n.z)))
    {

        alpha_11 = ((E_02.z * E_03.x) - (E_02.x * E_03.z)) / n.y;
        beta_11  = ((E_01.z * E_02.x) - (E_01.x * E_02.z)) / n.y;

    }else {

        alpha_11 = ((E_02.x * E_03.y) - (E_02.y * E_03.x)) / n.z;
        beta_11  = ((E_01.x * E_02.y) - (E_01.y * E_02.x)) / n.z;
    }

    // Compute the bilinear coordinates of the intersection point.
    if(std::abs<float>(alpha_11 - 1.0f) < QUAD_EPSILON){

        // Q is a trapezium.
        u = alpha;
        if(std::abs<float>(beta_11 - 1.0f) < QUAD_EPSILON){
             v = beta; // Q is a parallelogram.
        }else{
            v = beta / ((u * (beta_11 - 1.0f)) + 1.0f); // Q is a trapezium.
        }
    }else if(std::abs<float>(beta_11 - 1.0f) < QUAD_EPSILON){
        // Q is a trapezium.
        v = beta;
        u = alpha / ((v * (alpha_11 - 1.0f)) + 1.0f);
    }else{

        float A = 1.0f - beta_11;
        float B = (alpha * (beta_11 - 1.0f))
        - (beta * (alpha_11 - 1.0f)) - 1.0f;
        float C = alpha;
        float D = (B * B) - (4.0f * A * C);
        float Q = -0.5f * (B + ((B < 0.0f ? -1.0f : 1.0f) * std::sqrt(D)));
        u = Q / A;
        if((u < 0.0f) || (u > 1.0f)){
            u = C / Q;
        }
        v = beta / ((u * (beta_11 - 1.0f)) + 1.0f);
    }

    //Set intersection parameters
    res.shp = this;
    res.locWS = r(t);
    res.tHit = t;
    res.n = Normal(n.x, n.y, n.z);

    return true;
    */
}

BoundingBox Quad::objectSpaceBound()const{
    Assert(false);
    //TODO: Triangles store only 3 vertices in world space.
    //the architecture should be changed so that there is a base
    //abstract "Shape" class
    //which has a subclass:
    //TransformedShape
    //Triangles and Quad should inherit from Shape directly,
    //whereas Spheres and other things should
    //inherit from Transformed shape
    return BoundingBox();
}

BoundingBox Quad::worldSpaceBound()const{
    return BoundingBox(v_00, v_10).Union(
        BoundingBox(v_11, v_01) );
}


float Quad::surfaceArea()const{
    const Triangle A(v_00, v_01, v_10);
    const Triangle B(v_01, v_11, v_10);
    return A.surfaceArea() + B.surfaceArea();
}

Point Quad::randomPt()const{
    Vector u = v_10 - v_00;
    Vector v = v_01 - v_00;
    
    RNG rng; //TODO: Somehow factor out this stuff

    const float randU = rng.randomFloatOC(0.0f, 1.0f);
    const float randV = rng.randomFloatOC(0.0f, 1.0f);

    return v_00 +  (randU * u) + (randV * v);
}


