#ifndef IMAGE_TYPES_H
#define IMAGE_TYPES_H

#include "imageio/Image.h"

/**
 *  An HDR image is just a tristimulus buffer of type float.
 *  This means we have 32 bits per pixel(on any "normal" hardware).
 */
typedef Image<float,3> HDRImage;

/**
 *  Low dynamic range image(LDR) with consider as 8 bits per pixel.
 *  Yes, I know chars don't *have to be* 8 bits according to the C++ standard,
 *  but I don't plan on running my renderer on a microcontroller so I doubt this will
 *  be an issue....
 */
typedef Image<unsigned char,3> LDRImage;


#endif //IMAGE_TYPES_H
