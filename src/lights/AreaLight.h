#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include "Light.h"
#include "Quad.h"
#include "Assert.h"

//TODO: For now area lights are only quads
//I still need to implement sampling methods for other types
//(spheres, triangles) and then they can be lights too!

class AreaLight : public Light{
protected:
    Spectrum spd;
    Quad* shp;
    float area;
    float tPower;

public:

    AreaLight(const Spectrum& s, Quad* q) : 
        spd(s), shp(q), area(q->surfaceArea()), tPower(0.0f)
    
    {
        tPower = spd.integrate() / area;
    }

    virtual float totalPower()const{ return tPower;}

    virtual Spectrum getSPD()const{
        return spd / area;
    }

    virtual int getRecNumVisibilitySamples()const{ return 70; }

    virtual void getSamples(Point* arr, int nSamps)const{
        Assert(arr != NULL);
        for(int i = 0; i < nSamps; i++){
            arr[i]  = shp->randomPt();
        }
    }


    virtual Spectrum sampleIncidentRadiance(
        const Point& worldPoint,
        const Vector& normal,
        const Point& lightSamplePt)const
    {
        //TODO: Somehow reasonably assert that lightSamplePt is a sample of the light
        //TODO: If our sample is on the light source itself, ignore the dot product

        if(worldPoint == lightSamplePt){
            return getSPD();
        }else{
            Vector w(lightSamplePt - worldPoint);
            w.normalize();
            float mult = w.dot(normal) > 0.0f ? (1.0f / area) : 0.0f;
            return spd * mult;
        }


        /*
        Vector w(lightSamplePt - worldPoint);
        w.normalize();
        return
            spd *
            ((w.dot(normal) > 0.0f) ? (1.0f / area) : 0.0f);//Multiplier
        */
    }

    virtual const Shape* getGeom()const{
        return (const Shape*) shp;
    }


};


#endif //AREA_LIGHT_H
