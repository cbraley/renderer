#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>
#include <limits>
#include <cstdlib>
//--
#include "imageio/ImageTypes.h"

/**
 *  Functions for reading and writing both LDR and HDR images.
 */
namespace ImageIO{

    /**
     *  Write a PPM file.  Note that this is a "raw" ppm in that the raster
     *  is stored in binary.
     *
     *  @param fileName is the file path to write to.
     *  @param img is a pointer to the low dynamic range 3 channel image.
     *  @param comment is an optional comment with NO newlines.  Defaults to the empty string.
     *  @return true on success, false otherwise.
     */
    bool writePPM(const std::string& fileName, const LDRImage* const img,
        const std::string& comment = "");

    /**
     *  Write a PFM image to disk.
     *  Uses the endianness of the current system when writing the PFM.  Notes the endianess
     *  as specified in the PFM file format.
     *  @param fileName is the file path to write to.
     *  @param hdr is a pointer to the high dynamic range 3 channel image.
     *  @return true on success, false otherwise.
     */
    bool writePFM(const std::string& fileName, const HDRImage* const hdr);
}

#endif //IMAGE_IO_H
