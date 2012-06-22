#ifndef IMAGE_SENSOR_H
#define IMAGE_SENSOR_H

#include <vector>
//--
#include "utils/Assert.h"
#include "imsampling/ReconstructionFilter.h"
#include "color/Spectrum.h"
#include "imsampling/ImageSampler.h"
#include "imageio/ImageTypes.h"

/**
*  The image sensor models many kinds of radiance detector(CCD, CMOS, Film, etc).
*
*  Image sensor's have a width and height in pixels, a spectral response, and
*  a reconstruction filter.
*
*  The spectral response of a sensor denotes, for each
*  wavelength W, the probability that a photon of wavelength W will hit the sensor and
*  produce a hit(ie not be reflected).  This is the same as the "Quantum Effeciency"
*  of a sensor(QE).  QE curves are available from many camera manufacturers.
*  Essentially, this tells us "how responsive" the camera is to each wavelength of light.
*
*  The reconstruction filter describes what process is used to combine multiple samples into
*  a single pixel value.
*
*  The design choices represented in the "addSample" and "splatSample" methods are strongly
*  influenced by the elegant design found in PBRT 2 by Pharr and Humphreys.
*/
class ImageSensor{
public:

    /**
    *  Construct an image sensor.
    *
    *  @param pixWidth is the width(in pixels) of the sensor
    *  @param pixHeight is the height(in pixels) of the sensor
    *  @param spectralResponse is the quantum effeciency of the CCD.
    *  If this parameter is NULL, we assume that the device has a flat
    *  spectral response(ie QE(lambda) = 1.0). Ownership of this
    *  pointer is taken by the ImageSensor.
    *  @param rFilter is the reconstruction filter to use.
    *  Ownership of this pointer is taken by the ImageSensor.
    */
    ImageSensor(int pixWidth, int pixHeight, Spectrum* spectralResponse,
        ReconstructionFilter* rFilter);

    /**
    *  Destructor.
    */
    virtual ~ImageSensor();

    /**
     *  Add a sample to the image.  The sample will update the weighted averages in nearby pixels.
     *  Typically, this function should be used by rendering techniques like whitted stype raytracers,
     *  but not by bidirectional light transport algorithms(bidirectional path tracing, MLT) since these
     *  techniques generate light rays FROM the light source as well as from the camera.  These techniques
     *  should use the "splatSample" method.
     */
    void addSample(const ImageSampler::Sample& samp, Spectrum* spectrum);

    void splatSample(const ImageSampler::Sample& samp, Spectrum* spectrum);

    //TODO: Generalize this function so that we return a 2D array of pointers
    //to final spectra.  Then let the client code decide what to do with this spectral
    //data (convert to RGB, save as multispectral, etc)
    HDRImage* toImage(bool rgb = true)const;

    inline int getWidth()const{ return width; }
    inline int getHeight()const{ return height; }

private:

    float filterEvalHelper(float x, float y)const;

    //No copying allowed!
    ImageSensor(const ImageSensor& other);
    //No copying allowed!
    ImageSensor& operator=(const ImageSensor& rhs);

    void determineAffectedPixels(const ImageSampler::Sample& samp, int& xs, int& xe, int& ys, int& ye)const;

    inline void indicesToContinuous(int idxX, int idxY, float& contX, float& contY)const{
        contX = float(idxX) + 0.5f;
        contY = float(idxY) + 0.5f;
    }
    inline void continuousToIndices(float contX, float contY, int& idxX, int& idxY)const{
        idxX = int(contX);
        idxY = int(contY);
    }


    //Private pixel struct; holds XYZ data and a sum of filter weights
    typedef struct Pixel{
        float X,Y,Z;     //Running sum of XYZ color values
        float weightSum; //Running sum of reconstruction filter weights

        Pixel(float x, float y, float z) :
            X(x), Y(y), Z(z), weightSum(0.0f) {}

        Pixel():X(0.0f),Y(0.0f),Z(0.0f),weightSum(0.0f){}

        //Get the final pixel color
        inline void getFinalXYZ(float& x, float& y, float& z)const{
            const float weightInv = //Avoid div by 0 when inverting
                (weightSum == 0.0f) ? 0.0f : (1.0f / weightSum);
            x = X * weightInv;
            y = Y * weightInv;
            z = Z * weightInv;
        }
    }Pixel;

    ReconstructionFilter* filter;
    int width, height;
    Spectrum* resp;
    //Pixel* data; //TODO: Delete thsi and the Pixel class
    Image<float,4>* xyzw; //XYZ color components followed by weight

    inline Pixel getPixel(int x, int y)const{
        Pixel p;
        p.X         = *((*xyzw)(x,y,0));
        p.Y         = *((*xyzw)(x,y,1));
        p.Z         = *((*xyzw)(x,y,2));
        p.weightSum = *((*xyzw)(x,y,3));
        return p;
    }

    /// \brief Index into a 2D array allocated in 1D.
    /// Row major for now; might switch t blocked eventually for
    /// better cache locality...
    inline int index2Dto1D(int x, int y)const{ return (y * width) + x;}

    float Kval; //XYZ color normalization constant

    //Store the filter extent once, to avoid repeated virtual function calls
    float filterExtentX, filterExtentY;

    //Track error locations
    typedef struct EPixel{
        int x,y;
    }EPixel;
    std::vector<EPixel> errLocations;


    /// \brief Convert a spectrum ss to XYZ color, possibly accounting for the sensor QE.
    void spectrumToXYZWithQE(Spectrum* ss, float& X, float& Y, float& Z)const;
};

#endif //IMAGE_SENSOR_H
