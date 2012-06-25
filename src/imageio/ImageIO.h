#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>
#include <limits>
#include <cstdlib>
//--
#include "imageio/ImageTypes.h"

namespace ImageIO{

    /**
     *  Write a PPM file.  Note that this is a "raw" ppm in that the raster
     *  is stored in binary.
     */
    bool writePPM(const std::string& fileName, const LDRImage* const img,
        const std::string& comment = "");

    bool writePFM(const std::string& fileName, const HDRImage* const hdr,
        const std::string& comment = "");
}

#endif //IMAGE_IO_H
