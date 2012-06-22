#ifndef NO_ACCEL_STRUCTURE_H
#define NO_ACCEL_STRUCTURE_H

#include "accel/AccelStructure.h"
#include "utils/Assert.h"

/**
 *  Simple "non-accelerating" AccelStructure.  This class simply tests a given ray against
 *  every primitive in the scene for O(N) for intersecting one ray with N primitives.
 */
class NoAccelStructure : public AccelStructure{
protected:
    //Allow us to gaurentee that buildAccelStructure has been called first
    bool isSetup;
    std::vector<Shape*> scene;

public:

    NoAccelStructure() : isSetup(false) {}

    virtual void buildAccelStructure(const std::vector<Shape*>& sceneData){ scene = sceneData; isSetup = true; }

    virtual RaySurfIntersection getClosestIntersection(const Ray& r, bool& hitAnything)const{
        Assert(isSetup);

        RaySurfIntersection rClosest; //best hit so far
        RaySurfIntersection rCurr;    //the current intersection
        bool noHitSoFar = true;       //Have we hit anything yet?

        for(size_t i = 0; i < scene.size(); i++){
            const bool didHit = scene[i]->intersect(r, rCurr);
            if(didHit && (rCurr.tHit < rClosest.tHit || noHitSoFar) ){
                Assert(rCurr.tHit >= 0.0f);
                rClosest = rCurr;
                noHitSoFar = false;
            }
        }
        hitAnything = !noHitSoFar;
        return rClosest;
    }


    virtual bool doesIntersect(const Ray& r,
        float minDist, float maxDist,
        const Shape* ignoreShape)const
    {
        Assert(isSetup);

        RaySurfIntersection rCurr;    //the current intersection
        for(size_t i = 0; i < scene.size(); i++){
            const bool didHit = scene[i]->intersect(r, rCurr);
            if(didHit){
                //Check the position along the ray
                if(rCurr.tHit <= minDist || rCurr.tHit >= maxDist){
                    continue;
                }
                //Make sure we didn't hit the disallowed shape
                Assert(rCurr.shp != NULL);
                if(rCurr.shp == ignoreShape){
                    continue;
                }
                return true;
            }
        }
        return false;
    }


    virtual ~NoAccelStructure(){
        for(size_t i = 0; i < scene.size(); i++){
            delete scene[i]; scene[i] = NULL;
        }
    }

    virtual size_t getNumShapes()const{ return scene.size(); }

    virtual const Shape* getShape(size_t idx)const{
        Assert(idx < getNumShapes());
        return scene[idx];
    }



};

#endif //NO_ACCEL_STRUCTURE_H
