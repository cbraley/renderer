#include "color/ColorConvMat.h"

ColorConvMat::ColorConvMat(){
    //Set main diag to 1's...and the rest to 0's
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matrix[i][j] = (i==j ? 1.0f : 0.0f);
        }
    }
}

//Dev note:  when converting a raw listing of numbers pasted from the internet
//avoid annoying typing and use this regex
//    s/\s\(-\?\d.\d*\)/matrix[i][j] = \1f;/g
ColorConvMat::ColorConvMat(CommonColorConv convMat){
    switch(convMat){
        case XYZ_TO_ADOBE_1998_RGB:
            matrix[0][0] = 2.0413690f;  matrix[0][1] = -0.5649464f; matrix[0][2] = -0.3446944f;
            matrix[1][0] = -0.9692660f; matrix[1][1] = 1.8760108f;  matrix[1][2] = 0.0415560f;
            matrix[2][0] = 0.0134474f;  matrix[2][1] = -0.1183897f; matrix[2][2] = 1.0154096f;
            break;
        case ADOBE_1998_RGB_TO_XYZ:
            matrix[0][0] = 0.5767309f; matrix[0][1] = 0.1855540f; matrix[0][2] = 0.1881852f;
            matrix[1][0] = 0.2973769f; matrix[1][1] = 0.6273491f; matrix[1][2] = 0.0752741f;
            matrix[2][0] = 0.0270343f; matrix[2][1] = 0.0706872f; matrix[2][2] = 0.9911085f;
            break;
        case XYZ_TO_CIE_RGB:
            matrix[0][0] = 2.3706743f;  matrix[0][1] = -0.9000405f; matrix[0][2] = -0.4706338f;
            matrix[1][0] = -0.5138850f; matrix[1][1] = 1.4253036f;  matrix[1][2] = 0.0885814f;
            matrix[2][0] = 0.0052982f;  matrix[2][1] = -0.0146949f; matrix[2][2] = 1.0093968f;
            break;
        case CIE_RGB_TO_XYZ:
            matrix[0][0] = 0.4887180f; matrix[0][1] = 0.3106803f; matrix[0][2] = 0.2006017f;
            matrix[1][0] = 0.1762044f; matrix[1][1] = 0.8129847f; matrix[1][2] = 0.0108109f;
            matrix[2][0] = 0.0000000f; matrix[2][1] = 0.0102048f; matrix[2][2] = 0.9897952f;
            break;
        case XYZ_TO_sRGB:
            matrix[0][0] = 3.2404542f;  matrix[0][1] = -1.5371385f; matrix[0][2] = -0.4985314f;
            matrix[1][0] = -0.9692660f; matrix[1][1] = 1.8760108f;  matrix[1][2] = 0.0415560f;
            matrix[2][0] = 0.0556434f;  matrix[2][1] = -0.2040259f; matrix[2][2] = 1.0572252f;
            break;
        case sRGB_TO_XYZ:
            matrix[0][0] = 0.4124564f; matrix[0][1] = 0.3575761f; matrix[0][2] = 0.1804375f;
            matrix[1][0] = 0.2126729f; matrix[1][1] = 0.7151522f; matrix[1][2] = 0.0721750f;
            matrix[2][0] = 0.0193339f; matrix[2][1] = 0.1191920f; matrix[2][2] = 0.9503041f;
            break;
        case XYZ_TO_NTSC_RGB:
            matrix[0][0] = 1.9099961f;  matrix[0][1] = -0.5324542f; matrix[0][2] = -0.2882091f;
            matrix[1][0] = -0.9846663f; matrix[1][1] = 1.9991710f;  matrix[1][2] = -0.0283082f;
            matrix[2][0] = 0.0583056f;  matrix[2][1] = -0.1183781f; matrix[2][2] = 0.8975535f;
            break;
        case NTSC_RGB_TO_XYZ:
            matrix[0][0] = 0.6068909f;  matrix[0][1] = 0.1735011f; matrix[0][2] = 0.2003480f;
            matrix[1][0] = 0.2989164f;  matrix[1][1] = 0.5865990f; matrix[1][2] = 0.1144845f;
            matrix[2][0] = -0.0000000f; matrix[2][1] = 0.0660957f; matrix[2][2] = 1.1162243f;
            break;
        case IDENTITY_CONV:
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    matrix[i][j] = (i == j ? 1.0f : 0.0f);
                }
            }
            break;
        default:
            Assert(false);
            break;
    };
}



typedef const std::pair<const Spectrum*, const Spectrum*> iPair; //Normally I hate typedefs...
ColorConvMat::ColorConvMat(const Spectrum& fromB1, const Spectrum& fromB2, const Spectrum& fromB3,
    const Spectrum& toB1, const Spectrum& toB2, const Spectrum& toB3 )
{
    //...but when you are trying to show off C++-fu they can be needed to cut down on the 
    //verbosity.
    const iPair pairsp[3][3] = {
        { iPair(&fromB1, &toB1), iPair(&fromB1, &toB2), iPair(&fromB1, &toB3) },
        { iPair(&fromB2, &toB1), iPair(&fromB2, &toB2), iPair(&fromB2, &toB3) },
        { iPair(&fromB3, &toB1), iPair(&fromB3, &toB2), iPair(&fromB3, &toB3) }
    };
    const float NM_INT_MIN = 380.0f; //Integration limits
    const float NM_INT_MAX = 780.0f;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            matrix[i][j] = ( (*(pairsp[i][j].first)) *
                (*(pairsp[i][j].second))
                ).defIntegralTrapezoid(NM_INT_MIN, NM_INT_MAX);
        }
    }
}


