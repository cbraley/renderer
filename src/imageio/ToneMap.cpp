#include "ToneMap.h"
//--
#include <iostream>
#include <algorithm>
#include <iomanip>
//--
#include "Assert.h"
#include "Constants.h"
#include "RNG.h"

//#define TONEMAP_USING_BRIGHTNESS

static inline float brightness(const float r, const float g, const float b){
    return (r+g+b) / 3.0f;
}
static inline float clamp(const float v, const float minV, const float maxV){
    return (v <= minV ? minV : (v >= maxV ? maxV : v) );
}


static inline void setPixelHelperLDR(LDRImage* im,
    int x, int y,
    unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char* ar = (*im)(x,y,0);
    unsigned char* ag = (*im)(x,y,1);
    unsigned char* ab = (*im)(x,y,2);
    *ar = r;
    *ag = g;
    *ab = b;
}

namespace ToneMap{

LDRImage* toneMapMaxToWhite(HDRImage* in, bool minToBlack){

    //Alloc mem for result
    LDRImage* out = new LDRImage(in->width(), in->height());
    std::cout << "tmap 1 dims = " << in->width() << " by " << in->width() << std::endl;
    std::cout << "tmap 2 dims = " << out->width() << " by " << out->width() << std::endl;

    Assert(out != NULL);

    //Find min and max
#ifndef TONEMAP_USING_BRIGHTNESS
    float maxR, maxG, maxB, minR, minG, minB;
    maxR = maxG = maxB = MIN_FLOAT_VAL;
    minR = minG = minB = MAX_FLOAT_VAL;
    for(int i = 0; i < in->width(); i++){
        for(int j = 0; j < in->height(); j++){
            const float r = *((*in)(i,j,0));
            const float g = *((*in)(i,j,1));
            const float b = *((*in)(i,j,2));

            minR = std::min<float>(minR, r);
            maxR = std::max<float>(maxR, r);

            minG = std::min<float>(minG, g);
            maxG = std::max<float>(maxG, g);

            minB = std::min<float>(minB, b);
            maxB = std::max<float>(maxB, b);
        }
    }
    //Print info
    std::cout << "\tR values in range: [" << minR << ", " << maxR << "]" << std::endl;
    std::cout << "\tG values in range: [" << minG << ", " << maxG << "]" << std::endl;
    std::cout << "\tB values in range: [" << minB << ", " << maxB << "]" << std::endl;
    const float globalMin = std::min<float>( std::min<float>(minR, minG), minB);
    const float globalMax = std::max<float>( std::max<float>(maxR, maxG), maxB);
    std::cout << "\tGlobal range range: [" << globalMin << ", " << globalMax << "]" << std::endl;
#else
    float bmax, bmin;
    bmin = MAX_FLOAT_VAL; 
    bmax = MIN_FLOAT_VAL;
    for(int i = 0; i < in->width(); i++){
        for(int j = 0; j < in->height(); j++){
            const float r = *((*in)(i,j,0));
            const float g = *((*in)(i,j,1));
            const float b = *((*in)(i,j,2));
            const float brightnessVal = brightness(r,g,b);
            bmax = std::max<float>(brightnessVal, bmax);
            bmin = std::min<float>(brightnessVal, bmin);
        }
    }
    const float globalMax = bmax;
    const float globalMin = bmin;
#endif

    std::cout << "\tImage range = [" << std::setprecision(16) << 
        globalMin << ", " << std::setprecision(16) << 
        globalMax << "]" << std::endl;

    //Tone map and return
    toneMapLinear(in, minToBlack ? globalMin : 0.0f, globalMax, out);
    return out;
}

void toneMapLinear(HDRImage* in, float minV, float maxV, LDRImage* out){
    Assert(in != NULL && out != NULL);
    Assert(in->width() == out->width() && in->height() == out->height());
    Assert(minV <= maxV);

    //Tone map
    const float OUT_MAX = 255.0f;
    for(int i = 0; i < in->width(); i++){
        for(int j = 0; j < in->height(); j++){
            const float r = *((*in)(i,j,0));
            const float g = *((*in)(i,j,1));
            const float b = *((*in)(i,j,2));
#ifndef TONEMAP_USING_BRIGHTNESS
            const float newR = ((r - minV) / (maxV - minV)) * OUT_MAX;
            const float newG = ((g - minV) / (maxV - minV)) * OUT_MAX;
            const float newB = ((b - minV) / (maxV - minV)) * OUT_MAX;
#else
            const float currBrightness = brightness(r,g,b);
            const float mult = ((currBrightness - minV) / (maxV - minV)) * OUT_MAX;
            const float newR = clamp(r * mult, 0.0f, OUT_MAX);
            const float newG = clamp(g * mult, 0.0f, OUT_MAX);
            const float newB = clamp(b * mult, 0.0f, OUT_MAX);
            Assert(newR <= OUT_MAX);
            Assert(newG <= OUT_MAX);
            Assert(newB <= OUT_MAX);
#endif
            //(*out)(i,j,0) = (unsigned char)(newR);
            //(*out)(i,j,1) = (unsigned char)(newG);
            //(*out)(i,j,2) = (unsigned char)(newB);

            setPixelHelperLDR(out, i,j, 
                (unsigned char) (newR),
                (unsigned char) (newG),
                (unsigned char) (newB) );
        }
    }
}



LDRImage* toneMapPercent(float percToBlack, float percToWhite, HDRImage* in, int samples){
    Assert(in != NULL);
    Assert(samples == -1 || (samples >= 1) );
    Assert(percToWhite >= percToBlack);

    //If the user asked for more samples than is in the image, just get each pixel once
    samples = std::max<int>(samples, in->width() * in->height());

    //Allocate buffer to sort
    float* buf = new float[samples == -1 ? (in->width() * in->height()) : samples];

    //Populate buffer, maybe with random samples
    int cnt = 0;
    if(samples == in->width() * in->height()){ //Non-random
        for(int i = 0; i < in->width(); i++){
            for(int j = 0; j < in->height(); j++){
                const float r = *((*in)(i,j,0));
                const float g = *((*in)(i,j,1));
                const float b = *((*in)(i,j,2));
                buf[cnt++] = brightness(r,g,b);
            }
        }
    }else{ //Random
        RNG rng;
        for(; cnt < samples ;){
            const int i = rng.randomIntOC(0, in->width());
            const int j = rng.randomIntOC(0, in->height());
            
            const float r = *((*in)(i,j,0));
            const float g = *((*in)(i,j,1));
            const float b = *((*in)(i,j,2));
            buf[cnt++] = brightness(r,g,b);
        }
    }

    //Sort buffer and find percentiles
    std::sort(buf, buf + samples, std::less<float>() );

    float modPercBlack = clamp(percToBlack, 0.0f, 0.999f); //TODO: Make this hack cleaner
    float modPercWhite = clamp(percToWhite, 0.0f, 0.999f);
    float minMap = buf[(int)(modPercBlack * float(samples))];
    float maxMap = buf[(int)(modPercWhite * float(samples))];

    //Account for case where percentiles may be not in the range [0,1]
    //TODO: Fix
    //if(percToBlack < 0.0f || percToBlack > 1.0f){ minMap += (percToBlack - modPercBlack); }
    //if(percToWhite < 0.0f || percToWhite > 1.0f){ maxMap += (percToWhite - modPercWhite); }
    std::cout << "min = " << minMap << " max = " << maxMap << std::endl;
    Assert(maxMap >= minMap);

    //Free bufer mem
    delete[] buf; buf = NULL;

    //Allocate and populate output image
    LDRImage* out = new LDRImage(in->width(), in->height());
    toneMapLinear(in, minMap, maxMap, out);

    //Done
    return out;
}

void makeWhiteBorder(LDRImage* im){
    Assert(im != NULL);
    unsigned char BORDER_COLOR[3] = {255, 255,255};
    for(int i = 0; i < im->width(); i++){
        for(int j = 0; j < 3; j++){
            *((*im)(i,0             ,j)) = BORDER_COLOR[j]; 
            *((*im)(i,im->height()-1,j)) = BORDER_COLOR[j]; 
        }
    }
    for(int i = 0; i < im->height(); i++){
        for(int j = 0; j < 3; j++){
            *((*im)(0            ,i,j)) = BORDER_COLOR[j]; 
            *((*im)(im->width()-1,i,j)) = BORDER_COLOR[j]; 
        }
    }
}

}
