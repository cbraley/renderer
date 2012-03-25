#include "ImageSampler.h"
//--
#include "Assert.h"

ImageSampler::Sample::Sample(int idxX, int idxY, int numPixX, int numPixY) :
    x( float(idxX) + 0.5f), y( float(idxY) + 0.5f),
    err(false)
{
    Assert(idxX >= 0 && idxX < numPixX);
    Assert(idxY >= 0 && idxY < numPixY);
    Assert(numPixX > 0 && numPixY > 0);
}

ImageSampler::ImageSampler(int imageWidth, int imageHeight, int sampsPerPixel) :
    imWidth(imageWidth), imHeight(imageHeight), sampsPerPix(sampsPerPixel)
{
    Assert(imWidth > 0);
    Assert(imHeight > 0);
    Assert(sampsPerPix > 0);
}

int ImageSampler::getNumSamplesForWindow(int winLeft, int winRight, int winTop, int winBottom)const{
    Assert(winLeft   < winRight);
    Assert(winBottom > winTop);

    const int sampsX = winRight - winLeft + 1;
    const int sampsY = winBottom - winTop + 1;
    return sampsX * sampsY * sampsPerPix;
}

int ImageSampler::getNumSamplesForWholeImage()const{
    return getNumSamplesForWindow(0, imWidth-1, 0, imHeight-1);
}


ImageSampler::Sample* ImageSampler::genSamples()const{
    return getSamplesForSubwindow(0,imWidth-1,0,imHeight-1);
}


ImageSampler::Sample* ImageSampler::getSamplesForSubwindow(int winLeft, int winRight,
    int winTop, int winBottom)const
{
    const int N = getNumSamplesForWindow(winLeft, winRight, winTop, winBottom);
    ImageSampler:: Sample* ret = new ImageSampler::Sample[N];
    getSamplesForSubwindow(winLeft, winRight, winTop, winBottom, ret);
    return ret;
}
