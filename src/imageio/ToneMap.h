#ifndef TONE_MAP_H
#define TONE_MAP_H

#include "imageio/ImageTypes.h"

/**
 *  Simple tone mapping algorithms for mapping an HDR image down to a final 8 bit image.
 */
namespace ToneMap{

    /**
     *  The simplest possible tone mapping algorithm.
     *  Maps the maximum pixel found in image "in" to white.
     *
     *
     *  minToBlack allows you to map the darkest pixel in the image to black.  Off by default.
     */
    LDRImage* toneMapMaxToWhite(HDRImage* in, bool minToBlack = false);

    /**
     *  Linear tone mapping(just like max-to-white) but the user can specify their own
     *  minimum and maximum values so that one could map something less than the max to white, or
     *  something non-zero to black.
     *
     *  This is useful for dealing with images that have a few pixels that are much brighter
     *  or much darker than the rest.
     */
    void toneMapLinear(HDRImage* in, float minV, float maxV, LDRImage* out);



    /**
     *  TODO: IN CURRENT IMPLEMENTATION "perc" variables must be in range [0,1]
     *  Tone map an image by mapping given percentiles to black and others to white.
     *  Percentages are specified typically in the range [0,1], but percToWhite could be 2 for instance
     *  if you wished to map a theoretical pixel that is twice as bright as the brightest pixel to white.
     *  samples is an optional parameter that can be set to a non-zero integer that corresponds to how accurately
     *  to determine the image's percentiles.  When samples = -1, the default, we use all available pixels(slow).
     *  If samples was 10, we would use 10 random pixels from the image.  samples may only be -1, or a number ranging from
     *  [1, W*H] where W*H is the number of pixels in the image.
     */
    LDRImage* toneMapPercent(float percToBlack, float percToWhite, HDRImage* in, int samples = -1);


    /// \brief Give an image a 1 pixel wide white border.
    void makeWhiteBorder(LDRImage* im);
}


#endif //TONE_MAP_H
