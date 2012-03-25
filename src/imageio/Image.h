#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstring>
#include <limits>
#include <algorithm>
//--
#include "Assert.h"



/**
 *  Image class templated on pixel type.  The number of channels in
 *  the image(i.e monochromatic vs tristimulus vs multichannel) must
 *  be known at compile time.  Image width and height can be set at
 *  runtime.
 *
 *  Not copyable.  Client code will often
 *  maintain a pointer to an Image for this reason.
 */
template <typename PIXEL_T, int N_CHANNELS = 3>
class Image{
public:


    /**
     *  Create an image with a specified with and height.
     *  @param width is the width of the image.  Must be > 0.  Defaults to 512.
     *  @param height is the height of the image.  Must be > 0.  Defaults to 512.
     *  @param initVal is the initial value to fill the image with.
     *  Defaults to PIXEL_T(0.0).
     */
    Image(int width = 512, int height = 512, PIXEL_T initVal = PIXEL_T(0.0));

    /**
     *  Destructor.
     */
    virtual ~Image();

    /**
     *  Retrieve a pointer to the pixel value at spatial
     *  location (x,y) at color channel d.  Modifcation to
     *  the image typically occurs through this function.
     *
     *  If x or y are out of bounds, or d is not in {0,1,2}
     *  undefined behavior will occur and likely will crash the system.
     *
     *  @param w is the x location on the image.  Indices start at 0
     *   and x=0 indicates the left hand column.
     *  @param y is the y location on the image.  Indices start at 0
     *   and y=0 is the top row of the image.
     *  @d is the color index either, 0, 1, or 2.
     *   Typically, d=0 indicates red, d=1 indicates green, and d=2
     *   indicates blue.
     */
    PIXEL_T* operator()(int x, int y, int d);

    /**
     *  Read a pixel value.
     */
    void readPixel(int x, int y, PIXEL_T out[N_CHANNELS])const;

    PIXEL_T readPixelSingleComponent(int x, int y, int d)const;

    void getScanline(int y, PIXEL_T* out)const;

    /**
     *  Set all pixels to a specified value.
     */
    void fill(PIXEL_T val);

    /**
     *  @return image width.
     */
    int width()const;

    /**
     *  @return image height.
     */
    int height()const;

    void range(PIXEL_T& minVal, PIXEL_T& maxVal)const;

    PIXEL_T minVal()const;
    PIXEL_T maxVal()const;

    bool operator==(const Image& other)const;
    bool operator!=(const Image& other)const;


    //Define this to print all pixels whent the class is printed
    //to an std::ostream...if VERBOSE_PRINT_ALL_PIXELS is not defined,
    //only the image size, max, and min are printed
    #define VERBOSE_PRINT_ALL_PIXELS

    //TODO: Figure out how to get a template friend to go belo the class def...
    friend std::ostream& operator<<(std::ostream& os,
        const Image<PIXEL_T,N_CHANNELS>& hdr)
    {
        PIXEL_T minV, maxV;
        minV = maxV = 0.0f;
        hdr.range(minV, maxV);
        os << "HDR with dimensions (w,h) = (" << hdr.w << ", " << hdr.h <<
            "), " << N_CHANNELS << " channels, and range [" << minV << ", " << maxV << "]";
#ifdef VERBOSE_PRINT_ALL_PIXELS
        os << std::endl;
        os << "Raster: " << std::endl;
        for(int c = 0; c < N_CHANNELS; c++){
            os << "Color component " << c << std::endl;
            for(int y = 0; y < hdr.h; y++){
                os << "\t";
                for(int x = 0; x < hdr.w; x++){
                    os << hdr.readPixelSingleComponent(x,y,c) << " ";
                }
                os << std::endl;
            }
        }
#endif
        return os;
    }


    /**
     *  Get access to the internal pixel buffer.  With great power
     *  comes great responsibility!
     */
    PIXEL_T* getPixelBuf();


private:
    Image(const Image& other); //No copying!
    Image& operator=(const Image& rhs); //No copying!

    int w,h; //width and height
    PIXEL_T* pixels; //pixel array

    /// Return the total number of pixels in the buffer
    /// width * height * num_color_channels
    int totalPix()const;
};

template<typename PIXEL_T, int N_CHANNELS>
inline PIXEL_T* Image<PIXEL_T,N_CHANNELS>::operator()(int x, int y, int d){
    Assert(x >= 0 && x < w);
    Assert(y >= 0 && y < h);
    Assert(d >= 0 && d < N_CHANNELS);
    return pixels + ((y * w + x)*N_CHANNELS + d); //Row major
}


template<typename PIXEL_T, int N_CHANNELS>
inline void Image<PIXEL_T,N_CHANNELS>::readPixel(int x, int y, PIXEL_T out[N_CHANNELS])const{
    Assert(x >= 0 && x < w);
    Assert(y >= 0 && y < h);
    PIXEL_T* temp = pixels + ((y * w + x)*N_CHANNELS);
    out[0] = *temp++;
    out[1] = *temp++;
    out[2] = *temp++;
}


template<typename PIXEL_T, int N_CHANNELS>
inline PIXEL_T Image<PIXEL_T,N_CHANNELS>::readPixelSingleComponent(int x, int y, int d)const{
    Assert(x >= 0 && x < w);
    Assert(y >= 0 && y < h);
    Assert(d >= 0 && d < N_CHANNELS);
    const int index = ((y * w + x)*N_CHANNELS + d); //Row major
    return pixels[index];
}

template<typename PIXEL_T, int N_CHANNELS>
inline int Image<PIXEL_T,N_CHANNELS>::width()const{ return w; }

template<typename PIXEL_T, int N_CHANNELS>
inline int Image<PIXEL_T,N_CHANNELS>::height()const{ return h; }



template<typename PIXEL_T, int N_CHANNELS>
inline PIXEL_T Image<PIXEL_T,N_CHANNELS>::minVal()const{
    PIXEL_T a,b; a = b = 0.0f;
    range(a,b);
    return a;
}

template<typename PIXEL_T, int N_CHANNELS>
inline PIXEL_T Image<PIXEL_T,N_CHANNELS>::maxVal()const{
    PIXEL_T a,b; a = b = 0.0f;
    range(a,b);
    return b;
}

template<typename PIXEL_T, int N_CHANNELS>
inline int Image<PIXEL_T,N_CHANNELS>::totalPix()const{ return w * h * N_CHANNELS; }


template<typename PIXEL_T, int N_CHANNELS>
void Image<PIXEL_T,N_CHANNELS>::getScanline(int y, PIXEL_T* out)const{
    const int scanlineStart = ((y * w) * N_CHANNELS);
    memcpy((void*)out, (void*) (pixels + scanlineStart), w * N_CHANNELS * sizeof(PIXEL_T));
}


template<typename PIXEL_T, int N_CHANNELS>
Image<PIXEL_T,N_CHANNELS>::Image(int width, int height, PIXEL_T initVal) :
    w(width), h(height), pixels(NULL)
{
    //At compile time a compilation error is produced if the below
    //condition is not satisfied.
    COMPILE_TIME_ASSERT(N_CHANNELS>0);
    Assert(w > 0 && h > 0);
    pixels = new PIXEL_T[totalPix()];
    Assert(pixels != NULL);
    fill(initVal);
}

template<typename PIXEL_T, int N_CHANNELS>
Image<PIXEL_T,N_CHANNELS>::~Image(){
    delete[] pixels;
    pixels = NULL;
}


template<typename PIXEL_T, int N_CHANNELS>
void Image<PIXEL_T,N_CHANNELS>::fill(PIXEL_T val){
    const int N = totalPix();
    for(int i = 0; i < N; i++){
        pixels[i] = val;
    }
}

template<typename PIXEL_T, int N_CHANNELS>
void Image<PIXEL_T,N_CHANNELS>::range(PIXEL_T& minVal, PIXEL_T& maxVal)const{
    minVal = std::numeric_limits<PIXEL_T>::max();  //TODO: USE SOMETHING ELSE FROM CONSTANTS
    maxVal = -minVal; //FLT_MIN is the minimum POSTIVE number...this bug sucked

    const int totPix = totalPix();
    PIXEL_T* ptr = pixels;
    PIXEL_T* end = pixels + totPix;

    do{
        minVal = std::min<PIXEL_T>(minVal, *ptr);
        maxVal = std::max<PIXEL_T>(maxVal, *ptr);
    }while(++ptr != end);
}

template<typename PIXEL_T, int N_CHANNELS>
bool Image<PIXEL_T,N_CHANNELS>::operator==(const Image& other)const{
    return (this == &other) || //If same addr we know they are ==
    (
        //Short circuiting avoids the memcmp for images with
        //non-equal dimensions
        (
        (w == other.w && h == other.h) &&
        (memcmp((void*)pixels, (void*)other.pixels, w*h*N_CHANNELS*sizeof(PIXEL_T)) == 0)
        )
    );
}

template<typename PIXEL_T, int N_CHANNELS>
bool Image<PIXEL_T,N_CHANNELS>::operator!=(const Image& other)const{
    return !(*this == other);
}

template<typename PIXEL_T, int N_CHANNELS>
PIXEL_T* Image<PIXEL_T,N_CHANNELS>::getPixelBuf(){
    return pixels;
}

#endif //IMAGE_H
