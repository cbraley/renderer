#include "Camera.h"
//--
#include "Assert.h"
#include "MathUtils.h"

/*
Camera::Camera(const Transform& camToWorldTransform, float fieldOfViewDeg,
    float zNear, float zFar, float imWidth, float imHeight) :
    w2c(camToWorldTransform.inverse()), c2w(camToWorldTransform),
    fov(MathUtils::degreesToRads(fieldOfViewDeg)), clipNear(zNear), clipFar(zFar),
    imW(imWidth), imH(imHeight){

    Assert(clipNear <= clipFar);
    Assert(fov >= 0.0f);

    //Precompute focal length
    //fov is in radians
    focalLen = (imW / 2.0f) * tan(fov / 2.0f);
    Assert(focalLen > 0.0f);
}
*/


Camera::Camera(Transform& camToWorldTransform, float focalLength, float imWidth, float imHeight) :
    w2c(camToWorldTransform.inverse()), c2w(camToWorldTransform),
    focalLen(focalLength),
    imW(imWidth), imH(imHeight)
{
    Assert(focalLength > 0.0f);
    Assert(imW > 0.0f && imH > 0.0f);
    fov = 1.0f; //TODO: Compute properly
}

Camera::~Camera(){}

Ray Camera::generateRay(const float xp, const float yp)const{

    //Note that values just outside the range [0,1] are possible for xp and yp since
    //due to reconstruction filters for edge pixels potentially needing to sample somewhat
    //far away from each edge pixels center.
    //We assert that xp and yp lie inside [-2,2] since that is the farthest away they would
    //be for a single pixel image with normal reconstruction filters.
    Assert(xp >= -2.0f && xp <= 2.0f);
    Assert(yp >= -2.0f && yp <= 2.0f);

    //Compute position on camera image plane assuming camera at origin
    //looking down <0,0,1> with up vector <0,1,0>
    const float ipx = (imW * xp ) - (imW * 0.5f);
    const float ipy = (-imH * yp) + (imH * 0.5f);
    const float ipz = focalLen;

    //Create point on image plane
    const Point imagePlanePt(ipx, ipy, ipz);

    //Create ray through said point
    const Point camOriginCamSpace(0.0f, 0.0f, 0.0f);
    const Ray rayCamSpace(camOriginCamSpace, imagePlanePt - camOriginCamSpace);

    //Transform this camera space ray to world space and return the result
    return c2w(rayCamSpace);
}


std::ostream& operator<<(std::ostream& os, const Camera& cam){
    os << "Camera: " << std::endl;
    os << "\tfov(degrees) = " << MathUtils::radsToDegrees(cam.fov) << std::endl;
    os << "\tim_width = "     << cam.imW                           << std::endl;
    os << "\tim_height = "    << cam.imH                           << std::endl;
    os << "\tfocal_len = "    << cam.focalLen                      << std::endl;
    os << "\tPlacement = "    << cam.w2c(Point(0.0f, 0.0f, 0.0f))  << std::endl;
    return os;
}
