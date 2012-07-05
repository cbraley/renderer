#include "imsampling/JitteredSampler.h"
//--
#include "utils/Assert.h"

JitteredSampler::JitteredSampler(int imageWidth, int imageHeight,
    int sampsPerPixel, float jitterX, float jitterY) :
    ImageSampler(imageWidth, imageHeight, sampsPerPixel),
    jitX(jitterX), jitY(jitterY), rng()
{
    //If either jitter amount was -1, compute a good jitter s.t
    //the samples stay within a particular pixel.
    if(jitX == -1.0f){
        jitX = 0.5f;
    }
    if(jitY == -1.0f){
        jitY = 0.5f;
    }
    Assert(jitX >= 0.0f && jitY >= 0.0f);
}

void JitteredSampler::getSamplesForSubwindow(int winLeft, int winRight,
    int winTop, int winBottom, ImageSampler::Sample*& sampsGoHere)const{
    Assert(winLeft >= 0 && winLeft <= winRight);
    Assert(winTop >= 0 && winTop <= winBottom);
    Assert(winRight < imWidth);
    Assert(winBottom < imHeight);
    Assert(sampsGoHere != NULL);

    //Compute the number of total samples to allocate
    const int numSamps = getNumSamplesForWindow(winLeft, winRight,
        winTop, winBottom);

    //Loop through all pixels, generating sampsPerPix samples at each location
    int sampIdx = 0;
    for(int x = winLeft; x <= winRight; x++){
        for(int y = winTop; y <= winBottom; y++){

            //Generate a sample at the CENTER of the current pixel
            const Sample centeredSamp(x, y, getImageWidth(), getImageHeight());

            //Jitter each sample by a different amount
            for(int s = 0; s < sampsPerPix; s++){
                Sample currSamp(centeredSamp);

                //Jitter the sample by a random amount
                const float randX = (rng.randomFloatOO() * 2.0f * jitX) - jitX;
                const float randY = (rng.randomFloatOO() * 2.0f * jitY) - jitY;
                //const float randY = rng.randomFloatOO(-jitY, jitY);
                currSamp.x += randX;
                currSamp.y += randY;

                //Add to the sample array
                sampsGoHere[sampIdx++] = currSamp;
            }
        }
    }
}

