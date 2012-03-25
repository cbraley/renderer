#ifndef BRDF_GEOM_H
#define BRDF_GEOM_H

#include <cmath>
//--
#include "Vector.h"
#include "Constants.h"
#include "Assert.h"
#include "MathUtils.h"

//TODO: Conversion to from halfangle parameterization(i.e Symon's) to spherical angles

/**
 *  Functions for dealing with geometric issue relating to BRDFs.
 *  Below we have a list of defitions and terminology relavent for understanding
 *  what the functions in this code do.  Note that all the functions in this namespace
 *  are fully thread safe and reentrant.
 *
 *      BRDF Space: Orthonormal coordinate system where +z is the surface normal,
 *      and +x and +y are the tangent vectors.  All light vectors must be transformed
 *      into this coordinate system before BRDF shading calls can be executed.
 *      This contrasts with "world space" where tangent vectors, normals, etc. are
 *      typically expressed.
 *
 *      Functions are provided for converting to and from BRDF space and world space.
 *
 *      Spherical coordinate systems: BRDF computations require one to frequently
 *      refer to points on the unit hemisphere.  Points on the unit hemisphere can
 *      be expressed in terms of a "3D vector parameterization" or a
 *      "two angle parameterization."
 *
 *      3D Vector parameterization:  Points on the unit hemisphere are given by unit
 *      length 3D vectors.  One can think of these vectors as originating at the origin
 *      and pointing out so that their tip lies on the unit hemisphere.
 *
 *      Two angle parameterization: One can also parametrize the unit hemisphere using 2 angles.
 *      These angles are the elevation and azmiuth angle, given by $theta$ and $phi$, respectively.
 *
 *      Functions are provided for converting to from the 3D vector parameterization and the
 *      two angle parameterization.
 *
 *      Mirror reflection direction: Given an incident direction w_o and a normal n, the mirror
 *      reflection direction is the direction a perfect mirror scatters light in.
 *
 *      Code is provided to compute the mirror reflection direction.
 *
 */
namespace BRDFGeom{

    //Tested relatively rigourously, appears to work
    /**
     *  Given a normalized normal vector n; generate vectors t and b such that
     *  {n,t,b} are all orthonormal.  How t and b are generated are arbitrary.
     *
     *  This function will typically be used with isotropic BRDF's only, since the
     *  material has no "preferred" grain direction.
     *
     *  @param n is the normal.  Must be normalized.
     *  @param t is the tangent vector.  This is an output paramter.
     *  @param t is the tangent vector.  This is an output paramter.
     */
    void genTangentFrameForIsotropicBRDF(const Vector& n, Vector& t, Vector& b);


    //Tested relatively rigourously, appears to work
    /**
     *  Project a vector onto a plane defined by a normal.  Takes the vector v and projects
     *  it onto the plane having normal of n.  n MUST be normalized upon calling.  v need not be
     *  a normal vector.
     *
     *  Behavior is undefined when n == v, or when v is orthogonal to n!
     *
     *  @param v is the input vector.  Not necessarily normalized.
     *  @param n is the normal.  Must be normalized.
     */
    Vector projVectorOntoPlane(const Vector& v, const Vector& n);

    void genAnisotropicBRDFAngularCoords(const Vector& n, const Vector& wo, const Vector& wi,
        float& theta_in, float& phi_in, float& theta_out, float& phi_out);

    /**
     *
     *
     *
     */
    void genIsotropicBRDFAngularCoords(const Vector& n, const Vector& wo, const Vector& wi,
        float& theta_in, float& theta_out, float& phi_diff);


    /**
     *  Transform Vector v back from local coordinates to world space.
     *  The local "BRDF Space" is given by Vectors nWS, t1WS, and t2WS which form
     *  the normal and the two tangents, in world space.
     *
     *  Note: all vectors passed in MUST be unit vectors!
     *
     *  @param v is the vector we wish to transform(v is in BRDF space).
     *  @param nWS is the normal(world space).
     *  @param t1WS is a tangent(world space).
     *  @param t2WS is a tangent(world space).
     *  @return v in world space.
     */
    Vector vectorToWorldSpace(const Vector& v, const Vector& nWS, const Vector& t1WS, const Vector& t2WS);

    /**
     *  Transform Vector v from world space to local BRDF space.
     *  The local "BRDF Space" is given by Vectors nWS, t1WS, and t2WS which form
     *  the normal and the two tangents, in world space.
     *
     *  In BRDF space, the normal corresponds to +z (0,0,1), and the tangents correspond
     *  to +x and +y, thus forming an orthonormal basis.
     *
     *  Note: all vectors passed in MUST be unit vectors!
     *
     *  @param v is the vector we wish to transform(v is in world space).
     *  @param nWS is the normal(world space).
     *  @param t1WS is a tangent(world space).
     *  @param t2WS is a tangent(world space).
     *  @return v in world space.
     */
    Vector vectorToBRDFSpace(const Vector& v, const Vector& nWS, const Vector& t1WS, const Vector& t2WS);


    /**
     *  Get a vector in the BRDF coordinate system from
     *  angular variables theta and phi.  Returns a normalized vector.
     *
     *  @param theta is the elevation angle.  Argument is in radians!
     *  @param phi is the azimuth angle.  Argument is in radians!
     */
    Vector sphericalCoordToVector(float theta, float phi);

    /**
     *  Given a vector v, already in the BRDF coordinate system,
     *  this function finds the angular parameterization in terms
     *  of azimuth and elevation angles theta and phi.
     *  @param v is the vector in the BRDF coordinate system.  Must be normalized.
     *  @param theta is an output parameter.  The theta(elevation) angle in RADIANS.
     *  @param phi is an output parameter.  The phi(azimuth) angle in RADIANS.
     */
    void vectorToSphericalCoord(const Vector& v, float& theta, float& phi);

    /**
     *  For a vector omega_in already in the BRDF coordiante system,
     *  this routine computes a unit Vector that is in the direction
     *  of perfect mirror reflection.
     *  @param omega_in is the input light Vector in the BRDF coordinate system.
     *   Must be normalized.
     */
    Vector getMirrorReflVector(const Vector& omega_in);

}

//Inline function definitions -------------------------------------------------


inline Vector BRDFGeom::vectorToWorldSpace(const Vector& v,
    const Vector& nWS, const Vector& t1WS, const Vector& t2WS)
{
    Assert(v.getNormalized()      == v   );
    Assert(nWS.getNormalized()    == nWS );
    Assert(t1WS.getNormalized()   == t1WS);
    Assert(t2WS.getNormalized()   == t2WS);

    return Vector(   //This works because
        v.dot(t1WS), //the tangent frame (aka
        v.dot(t2WS), //the basis vectors for BRDF space
        v.dot(nWS )  //form an orthonormal CS.
    );
}

inline Vector BRDFGeom::vectorToBRDFSpace(const Vector& v,
    const Vector& nWS, const Vector& t1WS, const Vector& t2WS)
{
    Assert(v.getNormalized()      == v   );
    Assert(nWS.getNormalized()    == nWS );
    Assert(t1WS.getNormalized()   == t1WS);
    Assert(t2WS.getNormalized()   == t2WS);

    //Only works because tangent frame is orthonormal
    const Vector ret = (t1WS * v.x) + (t2WS * v.y) + (nWS * v.z);
    Assert(ret.getNormalized() == ret);
    return ret;
}


inline Vector BRDFGeom::sphericalCoordToVector(float theta, float phi){
    //Make sure angles are in [0, 2*Pi]
    return Vector(
        sinf(theta) * cosf(phi),
        sinf(theta) * cosf(PI_OVER_2_FLOAT - phi),
        cosf(theta)
    );
}

inline void BRDFGeom::vectorToSphericalCoord(const Vector& v, float& theta, float& phi){
    Assert(v == v.getNormalized()); //Arg must be normalized
    theta = acosf(v.z);
    //Note: atan2 is used here because the expression atanf(v.y / v.x)
    //won't give the right quadrant and can have divide by zero errors
    //when v.x is zero!
    phi = atan2(v.y, v.x);
}

inline Vector BRDFGeom::getMirrorReflVector(const Vector& omega_in){
    Assert(omega_in == omega_in.getNormalized());

    //TODO: If there is a bottleneck this can be optimized
    //include functionality for computing the mirror dir using
    //either angles as the argument or a vector....each case can
    //be optimized differently.

    //Get angular representation
    float theta, phi;
    theta = phi = 0.0f;
    BRDFGeom::vectorToSphericalCoord(omega_in, theta, phi);

    //Mirror reflection direction is rotated by 90 degrees (pi)
    return BRDFGeom::sphericalCoordToVector(theta, phi + PI_FLOAT);
}



inline void BRDFGeom::genTangentFrameForIsotropicBRDF(const Vector& n, Vector& t, Vector& b){
    Assert(n == n.getNormalized());
    //http://www.gamedev.net/topic/445164-how-to-get-a-direction-vector-perpendicular-to-a-direction-vector-/

    //Generate f vector
    //TODO: This could be sped up
    Vector f(0.0f, 0.0f, 0.0f);
    int minindex = -1;
    float minVal = MAX_FLOAT_VAL;
    float temp = 0.0f;
    for(int i = 0; i < 3; i++){
        temp = abs( n[i] );
        if(temp < minVal){
            minVal = temp;
            minindex = i;
        }
    }
    Assert(minindex == 0 || minindex == 1 || minindex == 2);
    f[minindex] = 1.0f;
    Assert(f == f.getNormalized());

    //Generate first arbitrary tangent
    t = n.cross(f);

    //Generate bitangent
    b = n.cross(t);

    //Make sure output is normalized
    t.normalize();
    b.normalize();
}


inline Vector BRDFGeom::projVectorOntoPlane(const Vector& v, const Vector& n){
    Assert(n == n.getNormalized());
    Assert(!(v.x == n.x && v.y == n.y && v.z == n.z));//Make sure vectors are not the same
    //Assert(v.dot(n) != 0.0f); //Maybe we should add an epsilon here, since for vectors nearly orthogonal the result will be numerically imprecise...

    const Vector temp = n * (v.dot(n));
    return Vector(v.x - temp.x, v.y - temp.y, v.z - temp.z);
}


inline void BRDFGeom::genAnisotropicBRDFAngularCoords(const Vector& n, const Vector& wo, const Vector& wi,
    float& theta_in, float& phi_in, float& theta_out, float& phi_out)
{
    //Make sure all vectors are normal vectors
    Assert(n  == n.getNormalized() );
    Assert(wo == wo.getNormalized());
    Assert(wi == wi.getNormalized());

    std::cout << "Inputs" << std::endl;
    std::cout << "\tn  = " << n  << std::endl;
    std::cout << "\two = " << wo << std::endl;
    std::cout << "\twi = " << wi << std::endl;

    //Make sure all vectors point outward.  By convention, all the vectors should point the same way as the normal
    //a dot product of less than 0 would indicate that they don't satisfy this requirement
    Assert(n.dot(wo) >= 0.0f);
    Assert(n.dot(wi) >= 0.0f);

    //We generate a new orthonormal frame for the vectors
    Vector t,b;
    BRDFGeom::genTangentFrameForIsotropicBRDF(n, t, b);

    //Validate orthonormality of new frame
    const float EPS_LOCAL = 1e-5;
    Assert( abs(n.dot(t)) < EPS_LOCAL );
    Assert( abs(n.dot(b)) < EPS_LOCAL );
    Assert( abs(b.dot(t)) < EPS_LOCAL );
    Assert( t == t.getNormalized()    );
    Assert( b == b.getNormalized()    );

    std::cout << "Tangent frame is: " << std::endl;
    std::cout << "\tn = " << n << std::endl;
    std::cout << "\tt = " << t << std::endl;
    std::cout << "\tb = " << b << std::endl;


    //Theta angles can be found without any type of transformation
    theta_in  = MathUtils::radsToDegrees(n.angle(wi) );
    theta_out = MathUtils::radsToDegrees(n.angle(wo) );

    //For phi angles we must project into the plane
    phi_in    = MathUtils::radsToDegrees(b.angle(BRDFGeom::projVectorOntoPlane(wi, n)));
    phi_out   = MathUtils::radsToDegrees(b.angle(BRDFGeom::projVectorOntoPlane(wo, n)));

    /*
    //Project original wi and wo vectors into this new orthonormal CS
    //TODO: This is wrong...trasnforming into "BRDF space" is incorrect.  Start here...
    const Vector wi_newspace = BRDFGeom::vectorToBRDFSpace(wi, n, t, b);
    const Vector wo_newspace = BRDFGeom::vectorToBRDFSpace(wo, n, t, b);
    std::cout << "New vectors: " << std::endl;
    std::cout << "\twi_new = " << wi_newspace << std::endl;
    std::cout << "\two_new = " << wi_newspace << std::endl;

    //Find angles to compute coordiantes
    theta_in  = MathUtils::radsToDegrees(n.angle(wi_newspace) );
    phi_in    = MathUtils::radsToDegrees(t.angle(wi_newspace) );

    theta_out = MathUtils::radsToDegrees(n.angle(wo_newspace) );
    phi_out   = MathUtils::radsToDegrees(t.angle(wo_newspace) );
    */

    //Make sure outgoing coords are within reasonable ranges
    Assert(theta_in  >= 0.0f && theta_in  <= 90.0f );
    Assert(theta_out >= 0.0f && theta_out <= 90.0f );
    Assert(phi_in    >= 0.0f && phi_in    <= 180.0f);
    Assert(phi_out   >= 0.0f && phi_out   <= 180.0f);
}

inline void BRDFGeom::genIsotropicBRDFAngularCoords(const Vector& n, const Vector& wo, const Vector& wi,
    float& theta_in, float& theta_out, float& phi_diff)
{
    //Make sure all vectors are normal vectors
    //Vectors must be normalized
    Assert(n  == n.getNormalized() );
    Assert(wo == wo.getNormalized());
    Assert(wi == wi.getNormalized());

    //Vectors should point out from the surface patch
    const float d1 = n.dot(wo);
    const float d2 = n.dot(wi);
    Assert(d1 >= 0.0f);
    Assert(d2 >= 0.0f);

    //Calculate phi_diff (we don't need the new CS for this)
    const Vector wo_plane = BRDFGeom::projVectorOntoPlane(wo, n).getNormalized();
    const Vector wi_plane = BRDFGeom::projVectorOntoPlane(wi, n).getNormalized();
    phi_diff = MathUtils::radsToDegrees(wo_plane.angle(wi_plane));

    //Calculate the theta values
    theta_out = MathUtils::radsToDegrees(n.angle(wo));
    theta_in  = MathUtils::radsToDegrees(n.angle(wi));
    /*
    Vector t,b;
    BRDFGeom::genTangentFrameForIsotropicBRDF(n, t, b);

    float phi_i = t.angle(wo_plane);
    float phi_o = t.angle(wi_plane);
    phi_diff = abs(phi_i - phi_o);
    */

}


#endif //BRDF_GEOM_H
