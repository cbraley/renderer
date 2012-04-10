#ifndef TEST_COLOR_UTILS_H
#define TEST_COLOR_UTILS_H

#include <UnitTest++.h>
#include <cstdlib>
#include <iostream>
//--
#include "ColorUtils.h"

using namespace ColorUtils;

SUITE(ColorUtilsTests){
   
    //Tested and it works
    /*
    TEST(HexStrGen){
        while(true){
            unsigned int r,g,b;
            r = g = b = 0;
            std::cin >> r;
            std::cin >> g;
            std::cin >> b;
            std::cout << std::endl;
            std::cout <<
                "RGB tuple (" << r << ", " << g << ", " << b << ") = " << 
                ColorUtils::getHexStr((unsigned char)r,(unsigned char)g,(unsigned char)b,"0x") << std::endl;
        }
    }
    */

    TEST(ConvMatIdentity){
        ColorConvMat I; //Identity transform, should do nothing!
        const float rgb[3] = {0.001f, 1.12f, 1.544f};
        float xyz[3] = {0.0f, 0.0f, 0.0f};
        I.apply(rgb, xyz);

        CHECK_CLOSE(xyz[0], rgb[0], 0.0f);
        CHECK_CLOSE(xyz[1], rgb[1], 0.0f);
        CHECK_CLOSE(xyz[2], rgb[2], 0.0f);
    }


    TEST(RgbXyzConv){
        const float COMPARE_EPS = 0.001f;
        srand((unsigned)time(0));
        for(int i = 0; i < 100; i++){
            float rRand, gRand, bRand;
            rRand = (float)rand()/(float)RAND_MAX;
            gRand = (float)rand()/(float)RAND_MAX;
            bRand = (float)rand()/(float)RAND_MAX;

            float rBack, gBack, bBack;
            float tempX, tempY, tempZ;
            ColorUtils::RGBToXYZ_sRGB(rRand, gRand, bRand,
                tempX, tempY, tempZ);
            ColorUtils::XYZToRGB_sRGB(tempX, tempY, tempZ,
                rBack, gBack, bBack);

            CHECK_CLOSE(rRand, rBack, COMPARE_EPS);
            CHECK_CLOSE(gRand, gBack, COMPARE_EPS);
            CHECK_CLOSE(bRand, bBack, COMPARE_EPS);

            //--

            float xRand, yRand, zRand;
            xRand = (float)rand()/(float)RAND_MAX;
            yRand = (float)rand()/(float)RAND_MAX;
            zRand = (float)rand()/(float)RAND_MAX;

            float xBack, yBack, zBack;
            float tempR, tempG, tempB;
            ColorUtils::XYZToRGB_sRGB(xRand, yRand, zRand,
                tempR, tempG, tempB);
            ColorUtils::RGBToXYZ_sRGB(tempR, tempG, tempB,
                xBack, yBack, zBack);

            CHECK_CLOSE(xRand, xBack, COMPARE_EPS);
            CHECK_CLOSE(yRand, yBack, COMPARE_EPS);
//            CHECK_CLOSE(zRand, zBack, COMPARE_EPS);
        }


    }

}

#endif //TEST_COLOR_UTILS_H
