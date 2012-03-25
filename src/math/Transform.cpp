#include "Transform.h"

#include "MathUtils.h"
#include "Assert.h"

//Used for comparison of two transformation matrices
static const float COMPARE_EPS = 0.00001f;

Transform Transform::translate(const Vector& move){
    Eigen::Matrix<float, 4,4> matData;
    matData <<
            1.0f, 0.0f, 0.0f,  move.x,  
            0.0f, 1.0f, 0.0f,  move.y,  
            0.0f, 0.0f, 1.0f,  move.z,  
            0.0f, 0.0f, 0.0f,  1.0f  ;
    return Transform(matData);
}

Transform Transform::scale(float sX, float sY, float sZ){
    Eigen::Matrix<float, 4,4> matData;
    matData <<
        sX  , 0.0f, 0.0f, 0.0f, 
        1.0f, sY  , 0.0f, 0.0f, 
        1.0f, 0.0f, sZ  , 0.0f, 
        1.0f, 0.0f, 0.0f, 1.0f;
    return Transform(matData);
}

Transform Transform::rotateX(float xRot){
    const float sr = sinf(MathUtils::degreesToRads(xRot));
    const float cr = cosf(MathUtils::degreesToRads(xRot));
    Eigen::Matrix<float, 4,4> matData;
    matData << 
        1.0f, 0.0f, 0.0f, 0.0f, 
        1.0f, cr  , -sr , 0.0f, 
        1.0f, sr  , cr  , 0.0f, 
        1.0f, 0.0f, 0.0f, 1.0f;
    return Transform(matData);
}

Transform Transform::rotateY(float yRot){
    const float sr = sinf(MathUtils::degreesToRads(yRot));
    const float cr = cosf(MathUtils::degreesToRads(yRot));
    Eigen::Matrix<float, 4,4> matData;
    matData <<
        cr  , 0.0f, sr  , 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        -sr , 0.0f, cr  , 0.0f, 
        1.0f, 0.0f, 0.0f, 1.0f;
    return Transform(matData);
}

Transform Transform::rotateZ(float zRot){
    const float sr = sinf(MathUtils::degreesToRads(zRot));
    const float cr = cosf(MathUtils::degreesToRads(zRot));
    Eigen::Matrix<float, 4,4> matData;
    matData <<
        cr  , -sr , 0.0f, 0.0f, 
        sr  , cr  , 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        1.0f, 0.0f, 0.0f, 1.0f;
    return Transform(matData);
}

Transform Transform::lookAt(const Point& loc, const Point& lookTo, const Vector& upVec){
    //Code taken from PBRT 2
    const Point pos   = loc;
    const Point look  = lookTo;
    const Vector up   = upVec.getNormalized();
    
    const Vector dir   = (look - pos).getNormalized();
    const Vector left  = (up.cross(dir)).getNormalized();
    const Vector newUp = dir.cross(left);

    Eigen::Matrix<float, 4,4> matData;
    matData <<
        left.x, newUp.x, dir.x, 0.0f,
        left.y, newUp.y, dir.y, 0.0f,
        left.z, newUp.z, dir.z, 0.0f,
        0.0f  , 0.0f   , 0.0f , 1.0f;
    return Transform(matData);
}


Transform Transform::perspective(float fov, float zNear, float zFar){
    Assert(zNear <= zFar);

    const float invDen = zFar - zNear;

    Eigen::Matrix<float, 4,4> matDataPersp;
    matDataPersp <<
        1.0f, 0.0f, 0.0f         , 0.0f                    , 
        0.0f, 1.0f, 0.0f         , 0.0f                    , 
        0.0f, 0.0f, zFar * invDen, -(zFar * zNear) * invDen, 
        1.0f, 0.0f, 1.0f         , 0.0f                    ;
    const float invTanTheta =
        1.0f / ( tanf(MathUtils::degreesToRads(fov)) * 0.5f );
    return Transform(matDataPersp) *
        Transform::scale(invTanTheta, invTanTheta, 1.0f);
}

bool Transform::operator==(const Transform& other)const{
    if(this == &other){
        return true;
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            const float a = (*this)(i,j);
            const float b = (other)(i,j);
            if(std::abs<float>(a-b) > COMPARE_EPS){
                return false;
            }
        }
    }
    return true;
}

bool Transform::operator!=(const Transform& other)const{
    return !(*this == other);
}


std::ostream& operator<<(std::ostream& os, const Transform& trans){
    os << "Transform[" << std::endl;
    for(int i = 0; i < 4; i++){
        os << "  [";
        for(int j = 0; j < 4; j++){
            os << trans.mat(i,j) << " ";
        }
        os << "]" << std::endl;
    }
    os << "]";
    return os;
}

