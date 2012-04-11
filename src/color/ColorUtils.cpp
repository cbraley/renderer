#include "ColorUtils.h"


std::string ColorUtils::getHexStr(unsigned char R,
    unsigned char G, unsigned char B,
    const std::string prefix)
{
    //Could have done this with the standard library but
    //its good to practice this kind of interview stuff...

    //Look up table for hex digits
    const char HEX_LUT[] =
        {'0','1','2','3','4','5','6','7','8','9',
        'A','B','C','D','E','F'};

    std::string res("000000");

    int strIndex = 0;
    unsigned char toConv[3] = {R,G,B};
    for(int i = 0; i < 3; i++){

        //Get current # in [0, 255]
        //convert to two chars for the out string
        unsigned char curr = toConv[i];
        char c2 = HEX_LUT[curr % 16];
        curr %= 16;
        char c1 = HEX_LUT[curr % 16];

        //Place them in the out string
        res[strIndex++] = c1;
        res[strIndex++] = c2;
    }

    //Return add the prefix
    return prefix + res;
}

