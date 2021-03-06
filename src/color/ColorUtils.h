#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <iostream>
#include <string>
#include <utility>
#include <string>
//--
#include "utils/Assert.h"
#include "color/ColorConvMat.h"
//#include "color/Spectrum.h"

/**
 *  Utility functions for conversion between various
 *  tristimulus color spaces. If more control or more
 *  "advanced operations" are needed, see ColorConvMat.h
 *  Also contains utilities for color format conversions.
 *
 *  Also includes utlity functions for generating RGB colors for monochromatic
 *  (single wavelength) light sources.  If you want more control over such an operation
 *  (such as using different RGB primaries) you can do it yourself using Spectrum.h and
 *  ColorConvMat.h
 *
 *  Also, contains code to convert from RGB to a hex-string for specifying colors.
 *
 *  @author Colin Braley
 *  @date Oct 7, 2010
 *  @revsion April 10, 2012
 */

namespace ColorUtils{

    //Common color conversion matrices
    static const ColorConvMat XYZ_TO_SRGB = ColorConvMat(ColorConvMat::XYZ_TO_sRGB);
    static const ColorConvMat SRGB_TO_XYZ = ColorConvMat(ColorConvMat::sRGB_TO_XYZ);

    /// \brief Convert XYZ to sRGB
    void XYZToRGB_sRGB(float X, float Y, float Z,
        float& R, float& G, float& B);

    /// \brief Convert sRGB to XYZ
    void RGBToXYZ_sRGB(float R, float G, float B,
        float& X, float& Y, float& Z);

    /// \brief Get a hex-string for a color
    /// Optionally you can prefix the string(maybe with "0x")
    std::string getHexStr(unsigned char R, unsigned char G, unsigned char B,
        const std::string prefix = "");
}

//Inline function defs --------------------------------------------------------
inline void ColorUtils::XYZToRGB_sRGB(float X, float Y, float Z,
    float& R, float& G, float& B)
{
    const float a[3] = {X,Y,Z};
    float b[3] = {R,G,B};
    XYZ_TO_SRGB.apply(a,b);
    R = b[0];
    G = b[1];
    B = b[2];
}

inline void ColorUtils::RGBToXYZ_sRGB(float R, float G, float B,
    float& X, float& Y, float& Z)
{
    const float a[3] = {R,G,B};
    float b[3] = {X,Y,Z};
    SRGB_TO_XYZ.apply(a,b);
    X = b[0];
    Y = b[1];
    Z = b[2];
}



#endif //COLOR_UTILS_H
