#ifndef COLOR_CONV_MAT_H
#define COLOR_CONV_MAT_H

#include <iostream>
//--
#include "color/Spectrum.h"
#include "utils/Assert.h"

/**
 *  Simple class that represents a 3x3 matrix that converts
 *  from one color space to another.  Allows simple construction
 *  of common matrices, like XYZ to RGB.  Also, allows construction
 *  of *arbitrary* matrices based on known primaries.
 *
 *  @author Colin Braley
 */
class ColorConvMat{
public:

    //Public for easy access
    float matrix[3][3]; //[rows][columns]

    /**
     *  Default to the identity transform.
     */
    ColorConvMat();

    //Used in a c-tor to allow simple construction of common color
    //conv matrices.
    enum CommonColorConv{
        XYZ_TO_ADOBE_1998_RGB, //D65 white point
        ADOBE_1998_RGB_TO_XYZ, //D65 white point
        XYZ_TO_CIE_RGB,
        CIE_RGB_TO_XYZ,
        XYZ_TO_sRGB,
        sRGB_TO_XYZ,
        XYZ_TO_NTSC_RGB,
        NTSC_RGB_TO_XYZ,
        IDENTITY_CONV //Identity matrix
    };

    /**
     *  Construct a conversion matrix from a set of common matrices.
     */
    ColorConvMat(CommonColorConv convMat);


    /**
     *  Construct a CS transform matrix that maps from the color space specified by basis
     *  functions:
     *      {fromB1, fromB2, fromB3}
     *  to a clor space specified by:
     *      {toB1, toB2, toB3}
     *
     *
     *  Cache this matrix to avoid recomputing it, as we are computing the projection of one basis
     *  onto another...in this case that necessitates computing 6 definite integrals.
     */
    ColorConvMat(const Spectrum& fromB1, const Spectrum& fromB2, const Spectrum& fromB3,
        const Spectrum& toB1, const Spectrum& toB2, const Spectrum& toB3 );




    /**
     *  Apply the specified matrix to convert from
     *  the color values in parameter "from" and plac the results
     *  in the parameter "to,"
     *  @param from = Argument with input color.
     *  @param to is a return-arg where the result is placed.
     */
    inline void apply(const float from[3], float to[3])const{
        //TODO: Verify these loops are being unrolled (do I need a pragma?)
        for(int i = 0; i < 3; i++){
            float accum = 0.0f;
            for(int j = 0; j < 3; j++){
                accum += matrix[i][j] * from[j];
            }
            to[i] = accum;
        }
    }

    /**
     *  Overloaded stream insertion operator.  Print a matrix to a std::ostream.
     */
    friend std::ostream& operator<<(std::ostream& os, const ColorConvMat& mat){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                os << mat.matrix[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
};



#endif //COLOR_CONV_MAT_H
