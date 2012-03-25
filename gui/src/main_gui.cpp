#include <iostream>
//--
#include <qapplication.h>
//--
#include "gui.h"
//--
#include "ImageTypes.h"
#include "RNGs.h"
#include "Assert.h"
#include "ImageIO.h"


/*
void imageTst(HDRImage* im, int numTests = 50000){

    float currMin, currMax;
    currMin = 0.0f;
    currMax = 0.0f;
    im->fill(0.0f);


    for(int test = 0; test < numTests; test++){

        //Random spatial indices
        int rx = RNG::randomInt(0, im->getWidth());
        int ry = RNG::randomInt(0, im->getHeight());

        //Random colors
        float rv = (float)((int)RNG::randomFloat(-10.0f, 10.0f));
        float gv = (float)((int)RNG::randomFloat(-10.0f, 10.0f));
        float bv = (float)((int)RNG::randomFloat(-10.0f, 10.0f));

        //Modify the image
        float* modmeR = (*im)(rx,ry,0);
        float* modmeG = (*im)(rx,ry,1);
        float* modmeB = (*im)(rx,ry,2);
        *modmeR = rv;
        *modmeG = gv;
        *modmeB = bv;


        //Make sure new pixel values agree
        float rgb[3] = {0.0f, 0.0f, 0.0f};
        im->readPixel(rx,ry, rgb);
        CHECK(rgb[0] == rv);
        CHECK(rgb[1] == gv);
        CHECK(rgb[2] == bv);

        CHECK(im->readPixelSingleComponent(rx,ry,0) == rv);
        CHECK(im->readPixelSingleComponent(rx,ry,1) == gv);
        CHECK(im->readPixelSingleComponent(rx,ry,2) == bv);
    }
}
*/

int main(int argc, char** argv){
    RNG::initializeRNGs();
    QApplication app(argc, argv);
    GUI gui;
    gui.show();
    return app.exec();
}
