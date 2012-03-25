#ifndef TRANSFORM_H
#define TRANSFORM_H

/**
 *  Transformation matrix class using 4x4 matrices.  Capable of transforming Points,
 *  Vectors, Rays, and Normals.
 *
 *  Static methods are used to create transforms of  specified types(rotation,
 *  scale, translate, etc) through the "named constructor idiom."
 *
 *  To apply a transformation to an object, the function call operator () is
 *  used.  This is inspired by PBRT.  Transformation concatenation is done using
 *  the * operator.
 */

#include <iostream>
//--
#include <Eigen/Core>
#include <Eigen/LU>
//--
#include "Vector.h"
#include "Normal.h"
#include "Point.h"
#include "Assert.h"
#include "Ray.h"
#include "BoundingBox.h"

/**
 *  4x4 Transformation matrix.
 */
class Transform{
public:
    /**
     *  Default to the identity transform.
     */
    Transform();

    /**
     *  Simple copy constructor.  Performs a deep copy.
     *  @param other is the matrix to copy.
     */
    inline Transform(const Transform& other);

    /**
     *  Construct a Transform with explicit arrays.
     *  @param mat is the matrix.
     */
    inline Transform(const float matData[4][4]);

    /**
     *  Construct a translation transformation.
     */
    static Transform translate(const Vector& move);

    //scale (1 = 100 percent scaling)
    static Transform scale(float sX, float sY, float sZ);

    //x rotation (degrees)
    static Transform rotateX(float xRot);

    //y rotation (degrees)
    static Transform rotateY(float yRot);

    //z rotation (degrees)
    static Transform rotateZ(float zRot);

    //look at transformation creates a transform that maps from camera local space to world space
    static Transform lookAt(const Point& loc, const Point& lookTo, const Vector& upVec);

    //Perspective transformation (useful in cameras)
    static Transform perspective(float fov, float zNear, float zFar);

    /**
     *  Construct a transform from an Eigen matrix.
     */
    inline Transform(const Eigen::Matrix<float,4,4>& eMat);

    /**
     *  Get the inverse of a given transformation.
     *  @return the inverse
     */
    inline Transform inverse()const;

    /**
     *  Read the (i,j)th element of a Transformation matrix.
     *  @param i is the row index (0 based)
     *  @param j is the column index (0 based)
     *  @return element (i,j)
     */
    inline float operator()(int i, int j)const;


    //Now we have functions for applying transformations
    Point operator()(const Point& p)const;
    Vector operator()(const Vector& v)const;
    Ray operator()(const Ray& r)const;
    Normal operator()(const Normal& n)const;
    
    //TODO: Instead, transform box basis vectors by tform, then project each of the points
    BoundingBox operator()(const BoundingBox& aabb)const;


    //Transformation composition.
    inline Transform operator*(const Transform& rhs)const;


    //IO
    friend std::ostream& operator<<(std::ostream& os, const Transform& trans);


    //Compares two transformations for equality using COMPARE_EPS defined
    //in Transform.cpp
    bool operator==(const Transform& other)const;
    bool operator!=(const Transform& other)const;


private:
    Eigen::Matrix<float, 4,4, Eigen::ColMajor> mat;

//Check for NaN values when debugging is on.
#ifdef NDEBUG
    inline bool hasNans()const{ return false; }
#else
    inline bool hasNans()const{
        //TODO: Use an Eigen "boolean reduction"
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(isnan(mat(i,j))){
                    return true;
                }
            }
        }
        return false;
    }
#endif
public:
    //See: http://eigen.tuxfamily.org/dox/TopicStructHavingEigenMembers.html
    //This is needed so that our Eigen 4x4 float matrix is aligned for SSE stuff...
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


//-----------------------------------------------------------------------------
//Inline function definitions -------------------------------------------------
//-----------------------------------------------------------------------------
inline Transform::Transform(){
    mat    = Eigen::Matrix<float,4,4>::Identity();
}

inline Transform::Transform(const Transform& other){
    mat    = other.mat;
    Assert(!hasNans());
}

inline Transform::Transform(const float matData[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            mat(i,j)    = matData[i][j];
        }
    }
    Assert(!hasNans());
}

inline Transform::Transform(const Eigen::Matrix<float,4,4>& eMat){
    mat = eMat;
}

inline Transform Transform::inverse()const{
    return Transform(mat.inverse());
    Assert(!hasNans());
}


inline float Transform::operator()(int i, int j)const{
    Assert(i >= 0 && i < 4);
    Assert(j >= 0 && j < 4);
    return mat(i,j);
}


inline Point Transform::operator()(const Point& p)const{
    const float x = p.x;
    const float y = p.y;
    const float z = p.z;

    const float xn = mat(0,0)*x + mat(0,1)*y + mat(0,2)*z + mat(0,3);
    const float yn = mat(1,0)*x + mat(1,1)*y + mat(1,2)*z + mat(1,3);
    const float zn = mat(2,0)*x + mat(2,1)*y + mat(2,2)*z + mat(2,3);
    const float hw = mat(3,0)*x + mat(3,1)*y + mat(3,2)*z + mat(3,3);

    if(hw == 1.0f){
        return Point(xn, yn, zn);
    }else{
        const float mlt = 1.0f / hw;
        return Point(xn * mlt, yn * mlt, zn * mlt);
    }
}

inline Vector Transform::operator()(const Vector& v)const{
    float x,y,z;
    x = v.x;
    y = v.y;
    z = v.z;
    return Vector(
        mat(0,0) * x + mat(0,1) * y + mat(0,2) * z,
        mat(1,0) * x + mat(1,1) * y + mat(1,2) * z,
        mat(2,0) * x + mat(2,1) * y + mat(2,2) * z
    );
}

inline Ray Transform::operator()(const Ray& r)const{
    //Simply apply the tform to the constituent parts of
    //the ray
    return Ray((*this)(r.getOrigin()), (*this)(r.getDir()), false);
}


inline Normal Transform::operator()(const Normal& n)const{
    //Note that this indexing looks funny because were
    //implicitly indexing into the transpose
    //...see PBRT v2 page 87 for why this is diffent that transforming a vector
    //the gist of it is that we need to make sure normal dot tangent = 0
    const Eigen::Matrix<float, 4,4> matInv = mat.inverse();
    return Normal(
        matInv(0,0) * n.x + matInv(1,0) * n.y + matInv(2,0) * n.z,
        matInv(0,1) * n.x + matInv(1,1) * n.y + matInv(2,1) * n.z,
        matInv(0,2) * n.x + matInv(1,2) * n.y + matInv(2,2) * n.z
    );
}


inline BoundingBox Transform::operator()(const BoundingBox& aabb)const{
    Point corners[8];
    aabb.getCorners(corners);
    BoundingBox ret;
    //Take the union of all 8 corners transformed into the new space
    for(int i = 0; i < 8; i++){
        ret.Union(
            (*this)(corners[i]) //Apply the transform to each point
        );
    }
    return ret;
}

inline Transform Transform::operator*(const Transform& rhs)const{
    const Eigen::Matrix<float, 4,4> mnew = mat * rhs.mat;
    return Transform(mnew);
}


#endif //TRANSFORM_H
