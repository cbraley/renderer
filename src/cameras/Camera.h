#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Ray.h"

/**
 *  Perspective camera class.
 */
class Camera{
public:

    /**
     *  Construct a perspective camera.  Use camToWorldTransform to place the
     *  camera in the scene.  By default, the camera looks down the +z axis
     *  with its COP at (0,0,0).
     *
     *  The "Camera" class and related subclasses only describe the geometric
     *  placement of the camera into the scene, and other geometric properties like
     *  field of view.  All radiometric properties for a particular camera are found
     *  in ImageSensor.  Other properties relating to the image sensor(CCD) itself, such
     *  as number of pixels, are found in ImageSensor as well.
     *
     *  Note that parameters zNear and zFar are currently ignored, but zFar must be >= zNear.
     *
     *  @param camToWorldTransform is a Transformation that places the camera in the scene.
     *  @fieldOfViewDeg is the cameras FOV in degrees.  Note that this is the FULL field of view,
     *   as opposed to the "half FOV" seen in some other graphics resources.
     *  @param zNear, zFar are for clipping planes (currently ignored).
     *  @param imWidth is the physical width of the image plane itself in units like m, NOT in pixels.
     *  @param imHeight is the physical height of the image plane itself in units like m, NOT in pixels.
     */
//TODO: Debug this
//    Camera(const Transform& camToWorldTransform, float fieldOfViewDeg,
//        float zNear, float zFar, float imWidth, float imHeight);

    Camera(Transform& camToWorldTransform, float focalLength, float imWidth, float imHeight);

    /**
     *  Destructor.
     */
    virtual ~Camera();

    /**
     *  Generate ray at point (xp, yp) in the image plane.
     *  xp and yp should be in the range [0,1].
     */
    virtual Ray generateRay(const float xp, const float yp)const;

    friend std::ostream& operator<<(std::ostream& os, const Camera& cam);

private:
    Transform w2c;           //World to camera transform
    Transform c2w;           //Camera to world transform
    float fov;               //Field of view(stored in radians)
    float imW, imH;          //Image dims
    float focalLen;          //Focal length
};


#endif //CAMERA_H
