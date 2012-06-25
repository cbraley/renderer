#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "color/Spectrum.h"
#include "math/Transform.h"
#include "lights/Light.h"
#include "utils/Assert.h"
#include "utils/Constants.h"

/**
 * Point light with no falloff and an arbitrary user-specified SPD.
 */
class PointLight : public Light{
protected:
    Spectrum spd;
    Transform o2w;
    Point wsLoc;
    float tPower;

public:

    //Default to pure white light at the origin
    PointLight() : spd(Spectrum::CIE_ILLUM_E), o2w(Transform()){
        wsLoc = o2w(Point(0.0f,0.0f,0.0f));
        tPower = spd.integrate();
    }

    //Pass in a spectral power distribution and a custom transform
    PointLight(const Spectrum& s, const Transform& lightToWorld) :
        spd(s), o2w(lightToWorld)
    {
        wsLoc = o2w(Point(0.0f,0.0f,0.0f));
        tPower = spd.integrate();
    }

    virtual Spectrum getSPD()const{ return spd; }

    virtual int getRecNumVisibilitySamples()const{ return 1; }

    virtual void getSamples(Point* arr, int nSamps)const{
        Assert(nSamps > 0 && arr != NULL);
        for(int i = 0; i < nSamps; i++){
            arr[i] = wsLoc;
        }
    }

    virtual float totalPower()const{ return tPower; }

    virtual Spectrum sampleIncidentRadiance(
        const Point& worldPoint,
        const Vector& normal,
        const Point& lightSamplePt)const
    {
        return spd / (Constants::FOUR_PI_FLOAT);
    }


};


#endif //POINT_LIGHT_H
