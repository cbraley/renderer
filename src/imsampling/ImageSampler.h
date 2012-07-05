#ifndef IMAGE_SAMPLER_H
#define IMAGE_SAMPLER_H


/**
 *  Defines an interface that all ImageSampler's must implement.
 *  This base class implements simple methods to make less work for the derived classes.
 *
 *  Image Samplers are responsible for choosing sample locations for a
 *  pixel grid.
 */
class ImageSampler{
protected:
    int imWidth, imHeight, sampsPerPix;
public:

    /**
     *  Sample struct; represents a single image sample.  Samples have floating point coordinate 
     *  values (x,y) in the range [0, NUM_PIX) in each direction.  
     */
    typedef struct Sample{
    public:
        float x, y;
        bool err; //TODO: This is here only for debugging.  Remove later!

        /**
         *  Constructor.
         *  @param xP is the x position of the sample. Defaults to 0.0f.
         *  @param yP is the y position of the sample. Defaults to 0.0f.
         */
        Sample(float xP = 0.0f, float yP = 0.0f): x(xP), y(yP), err(false)  {}

        /**
         *  Construct a sample that lies at the center of pixel
         *  with raster coordiantes idxX, idxY in an image that has
         *  numPixX and numPixY samples.
         */
        Sample(int idxX, int idxY, int numPixX, int numPixY);

    }Sample;

    ImageSampler(int imageWidth, int imageHeight, int sampsPerPixel);

    /// \brief Get image width.
    int getImageWidth()const;
    ///\brief Get image height.
    int getImageHeight()const;
    /// \brief Get image number of samples per pixel.
    int getSamplesPerPixel()const;

    /**
     *  Generate samples for a subwindow with INCLUSIVE indices.  So for a call:
     *  getNumSamplesForWholeImage(0,10,0,10) assuming 1 sample per pixel would return
     *  11 * 11 = 121
     */
    int getNumSamplesForWindow(int winLeft, int winRight, int winTop, int winBottom)const;
    int getNumSamplesForWholeImage()const;

    /**
     *  Get an array of numSamples samples within a rectangular region
     *  of the image plane.  Note that, in the image plane, (0,0) is the
     *  upper left corner.
     *
     *  The returned array of samples must be delete[]'d by the calee
     *  upon completion.
     */
    virtual Sample* getSamplesForSubwindow(int winLeft, int winRight,
        int winTop, int winBottom)const;


    /**
     *  Genrate samples for the windoe just like the other getSamplesForSubwindow.
     *  However, this function places the results into an already allocated
     *  array sampsGoHere.  Allocating enough memory beforehand is the 
     *  responsibility of the caller.
     */
    virtual void getSamplesForSubwindow(int winLeft, int winRight,
        int winTop, int winBottom, Sample*& sampsGoHere)const = 0;

    /**
     *  Generate samples for the whole image.
     *
     *  The returned array of samples must be delete[]'d by the callee
     *  upon completion.
     */
    virtual Sample* genSamples()const;
    
};


inline int ImageSampler::getImageWidth()const{ return imWidth; }
inline int ImageSampler::getImageHeight()const{ return imHeight; }
inline int ImageSampler::getSamplesPerPixel()const{ return sampsPerPix; }

#endif //IMAGE_SAMPLER_H
