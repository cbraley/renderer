#include "imsampling/ImageSensor.h"
//--
#include <iomanip>
//--
#include "color/ColorUtils.h"
#include "parallel/AtomicOps.h"

//TODO: Remove this before public release, for debugging only
//...trying to track down bizarre pixels...
//Maximum number of errors to store information for
static const size_t MAX_ERRORS = 1024;
#define IS_MARK_ERROR_PIXELS
#define IS_PRINT_ERROR_PIXELS


//Helper functions-------------------------------------------------------------

void ImageSensor::spectrumToXYZWithQE(Spectrum* ss,
    float& X, float& Y, float& Z)const{
    X = Y = Z = 0.0f;
    Spectrum resultSpec = *ss;
    if(resp != NULL){
        resultSpec = (*ss) * (*resp);
        //resultSpec *= (*resp);
        //TODO: Potentially overload *= in Spectrum for speed
    }
    resultSpec.toXYZ(X,Y,Z,Kval);
}

void ImageSensor::determineAffectedPixels(const ImageSampler::Sample& samp,
    int& xs, int& xe, int& ys, int& ye)const{
    const float halfW = filterExtentX * 0.5f;
    const float halfH = filterExtentY * 0.5f;
    continuousToIndices(samp.x - halfW, samp.y - halfH, xs, ys);
    continuousToIndices(samp.x + halfW, samp.y + halfH, xe, ye);
}


float ImageSensor::filterEvalHelper(float x, float y)const{
    //TODO: Do perf tests to determine if this is worthwhile
    //
    //Currently, we just evaluate the filter naively here.  However,
    //if this turns out to be a bottlebeck we could evaluate the filter
    //ahead of time for many different values and create a lookup table.
    //
    //This would allow us to avoid the many virtual function calls.
    return filter->evalFilter(x,y);
}

//----------------------------------------------------------------------------

ImageSensor::ImageSensor(int pixWidth, int pixHeight,
    Spectrum* spectralResponse, ReconstructionFilter* rFilter) :
    filter(rFilter), width(pixWidth), height(pixHeight),
    resp(spectralResponse), Kval(0.0f),
    filterExtentX(filter->extentX()), filterExtentY(filter->extentY())
{
    Assert(width > 0 && height > 0);
    Assert(filter != NULL);

    xyzw = new Image<float,4>(width, height, 0.0f);
    Assert(xyzw != NULL);

    //TODO: Expose choice to user to 10 deg vs 2 deg CIE std observer
    //and temperature of dominant light source for whitebalancing
    Spectrum yLum(Spectrum::CIE_Y_10_DEG);
    Kval = 100.0f / (yLum.defIntegralTrapezoid(yLum.getMinNm(), yLum.getMaxNm()));
}


ImageSensor::~ImageSensor(){
    delete filter;  filter = NULL;
    delete resp;    resp   = NULL; //resp may be NULL but delete NULL is OK
    delete xyzw;    xyzw   = NULL;
}


void ImageSensor::addSample(const ImageSampler::Sample& samp, Spectrum* spectrum){
    Assert(spectrum != NULL);

    //Spectrum must be multiplied by our camera's response
    //and then converted to XYZ color.  Note that if the QE of the
    //sensor is NULL (resp == NULL) we assume a flat spectral response.
    float X,Y,Z; X = Y = Z = 0.0f;
    spectrumToXYZWithQE(spectrum, X,Y,Z);

    //Determine the affected pixels
    int xs,ys,xe,ye;
    xs = ys = xe = ye = -1;
    determineAffectedPixels(samp, xs, xe, ys, ye);

    for(int x = xs; x <= xe; x++){
        for(int y = ys; y <= ye; y++){

            //Skip this pixel if we are off the image plane
            //This can occur due to the extent of a reconstruction
            //filter being > half a pixel width and pushing a sample slightly
            //over the edge of the image plane
            const bool oob = x < 0 || y < 0 || x >= getWidth() || y >= getHeight();
            if(oob){ continue; }

            //Compute the filter weight at this location for the current pixel
            //    Compute the center of the pixel in continuous coords
            float pixelCenterX, pixelCenterY;
            pixelCenterX = pixelCenterY = -1.0f;
            indicesToContinuous(x, y, pixelCenterX, pixelCenterY);
            //    Compute filter weight based on distance from pixel center
            const float xDiff = samp.x - pixelCenterX;
            const float yDiff = samp.y - pixelCenterY;
            const float weight = filterEvalHelper(xDiff, yDiff);

            //Make sure filter weight >= 0
            const float MIN_ALLOWED_WEIGHT = 0.0f;
            Assert(weight >= MIN_ALLOWED_WEIGHT);

            //Update the pixel in question
            float* xPtr = (*xyzw)(x, y, 0);
            float* yPtr = (*xyzw)(x, y, 1);
            float* zPtr = (*xyzw)(x, y, 2);
            float* wPtr = (*xyzw)(x, y, 3);

            AtomicOps::AtomicAdd(xPtr, X * weight);
            AtomicOps::AtomicAdd(yPtr, Y * weight);
            AtomicOps::AtomicAdd(zPtr, Z * weight);
            AtomicOps::AtomicAdd(wPtr,     weight);


            //TODO: Once we parallelize, we will want to update these XYZ values with
            //atomic operations, or consider a mutex locking around individual tiles in the pixel buffer
            //likely the atomics choice is the best
            /*
            const int pixel1DIndex = index2Dto1D(x,y);
            Pixel pix = data[pixel1DIndex];
            pix.X += X * weight;
            pix.Y += Y * weight;
            pix.Z += Z * weight;
            pix.weightSum += weight;
            data[pixel1DIndex] = pix;
            */
        }
    }
}

void ImageSensor::splatSample(const ImageSampler::Sample& samp, Spectrum* spectrum){
    Assert(spectrum != NULL);

    //Spectrum must be multiplied by our camera's response
    //and then converted to XYZ color
    float X,Y,Z; X = Y = Z = 0.0f;
    spectrumToXYZWithQE(spectrum, X,Y,Z);

    //Compute discrete indices
    int x,y; x = y = -1;
    continuousToIndices(samp.x, samp.y, x, y);

    //Skip this pixel if we are off the image plane
    //This can occur due to the extent of a reconstruction
    //filter being > half a pixel width and pushing a sample slightly
    //over the edge of the image plane
    const bool oob = x < 0 || y < 0 || x >= getWidth() || y >= getHeight();
    if(oob){ return; }

    //Update the pixel buffer
    float* xPtr = (*xyzw)(x, y, 0);
    float* yPtr = (*xyzw)(x, y, 1);
    float* zPtr = (*xyzw)(x, y, 2);

    AtomicOps::AtomicAdd(xPtr, X);
    AtomicOps::AtomicAdd(yPtr, Y);
    AtomicOps::AtomicAdd(zPtr, Z);

    //TODO: When parallelizing this will need to change.  See addSample()
    /*
    const int pixel1DIndex = index2Dto1D(x,y);
    Pixel pix = data[pixel1DIndex];
    pix.X += X;
    pix.Y += Y;
    pix.Z += Z;
    data[pixel1DIndex] = pix;
    */
}

inline static void setPixelHelper(HDRImage* im, int xc, int yc, int dc, float val){
    float* ptr = (*im)(xc,yc,dc);
    *ptr = val;
}

HDRImage* ImageSensor::toImage(bool rgb)const{
    HDRImage* ret = new HDRImage(width, height);
    ret->fill(0.0f);

    const float ERR_OUT_COMP1 = 255.0f;
    const float ERR_OUT_COMP2 = 0.0f;
    const float ERR_OUT_COMP3 = 0.0f;

    //TODO: Maybe parallelize?
    //test to see how long spectrum to XYZ conversion is....
    for(int w = 0; w < width; w++){
        for(int h = 0; h < height; h++){
            
            Pixel currPix = getPixel(w,h);
            
            //Normalize the pixel's XYZ values
            float x,y,z;
            x = y = z = 0.0f;
            currPix.getFinalXYZ(x,y,z);

            //Potentially convert to RGB
            float ret1, ret2, ret3;
            ret1 = ret2 = ret3 = 0.0f;
            if(rgb){
                ColorUtils::XYZToRGB_sRGB(x,y,z,
                    ret1,ret2,ret3);
            }else{ //...or maybe give back XYZ
                ret1 = x;
                ret2 = y;
                ret3 = z;
            }

            //Set pixel in output buffer
            setPixelHelper(ret, w, h, 0, ret1);
            setPixelHelper(ret, w, h, 1, ret2);
            setPixelHelper(ret, w, h, 2, ret3);
        }
    }

    //Deal with errors by potentially either marking pixels
    //and/or printing info
    if(errLocations.size() >= MAX_ERRORS){
        std::cerr << "WARNING - Encountered more than: " << MAX_ERRORS
            << " error pixels when rendering." << std::endl;
    }
    for(size_t i = 0; i < errLocations.size(); i++){
        const int x = errLocations[i].x;
        const int y = errLocations[i].y;
#ifdef IS_PRINT_ERROR_PIXELS
        std::cerr << "WARNING - Error at pixel: (" << x << ", " << y << ")" << std::endl;
#endif
#ifdef IS_MARK_ERROR_PIXELS
        setPixelHelper(ret, x, y, 0, ERR_OUT_COMP1);
        setPixelHelper(ret, x, y, 1, ERR_OUT_COMP2);
        setPixelHelper(ret, x, y, 2, ERR_OUT_COMP3);
#endif
    }

    return ret;
}



