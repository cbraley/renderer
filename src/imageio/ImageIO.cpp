#include "imageio/ImageIO.h"
//--
#include <fstream>
#include <sstream>
//--
#include "utils/Assert.h"
#include "utils/Bits.h"


static inline std::string getNewlineStr(){
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
}

bool ImageIO::writePPM(const std::string& fileName, const LDRImage* const img,
    const std::string& comment)
{
    Assert(img != NULL);

    //Make sure the comment string has no newlines
    //TODO: We really shoud ensure that there are no newlines for
    //any platform's newline characters(ie no Windows, Linux, or Mac newlines)
    if(comment.find(getNewlineStr()) != std::string::npos){
        //Comment string contained newlines!
        return false;
    }

    std::fstream f(fileName.c_str(), std::fstream::out);
    if(!f){ //Make sure stream opened OK
        f.close();
        return false;
    }

    //Currently, we always assume that our image covers [0, 255]
    const int maxVal = 255;

    //PPM header
    f << "P6 "           << " " << std::endl;   //Magic number
    if(!comment.empty()){                       //Potentially write
        f << "#" << comment << std::endl;       //user's comment
    }
    f << img->width( ) << " " << std::endl << //Raster width
         img->height() << " " << std::endl << //Raster height
         maxVal               << std::endl;   //Maximum value in image
    f.close();

    //Open up a stream to write the raster data in binary
    std::fstream fRaster(fileName.c_str(),  //Need to have app for "append" and "ate" for at-end
        std::fstream::out | std::fstream::binary | std::fstream::app  | std::fstream::ate);
    if(!fRaster){ //Make sure stream opened OK
        f.close();
        return false;
    }

    //PPM raster
    const int rows = img->height();
    unsigned char* buf = new unsigned char[img->width() * 3];
    for(int r = 0; r < rows; r++){
        img->getScanline(r, buf);
        fRaster.write((const char*) buf, img->width() * 3);
    }
    delete[] buf; buf = NULL;

    //All done!
    fRaster.close();
    return true;
}



static bool writePFMHeader(
    const std::string& fileName,
    const HDRImage* const hdr)
{
    //See: http://netpbm.sourceforge.net/doc/pfm.html

    //Open an fstream in ASCII mode
    std::fstream fs(fileName.c_str(), std::fstream::out);
    if(! fs.good() ){
        fs.close();
        return false;
    }

    //Write the header
    const std::string COLOR_IDENTIFIER_LINE("PF");
    //const std::string GRAYSCALE_IDENTIFIER_LINE("Pf");
    const std::string LITTLE_ENDIAN_STR("-1.000000");
    const std::string BIG_ENDIAN_STR("1.000000");

    fs << COLOR_IDENTIFIER_LINE << std::endl;
    fs << hdr->width() << " " << hdr->height() << std::endl;
    fs << (Bits::isBigEndian() ? BIG_ENDIAN_STR : LITTLE_ENDIAN_STR) << std::endl;


    //Return whether the read went OK
    bool ret = fs.good();
    fs.close();
    return ret;
}
    
static bool writePFMRaster(
    const std::string& fileName,
    const HDRImage* const hdr)
{
    //See: http://netpbm.sourceforge.net/doc/pfm.html
    std::fstream fs(fileName.c_str(),
        std::fstream::out |    //Output stream
        std::fstream::binary | //Binary data
        std::fstream::ate |    //"AT End"(put data after header)
        std::fstream::app      //All writes are appends
    );
    if(! fs.good() ){
        fs.close();
        return false;
    }

    //Write the raster, row by row
    const int numCols = hdr->width();
    float* rowBuf = new float[numCols * 3];
    for(int y = 0; y < hdr->height(); y++){

        //Copy data for a scanline
        hdr->getScanline(y, rowBuf);

        //Write a scanline
        fs.write((const char*)rowBuf, sizeof(float) * numCols * 3);
    }

    delete[] rowBuf; rowBuf = NULL;

    //Return whether the read went OK
    bool ret = fs.good();
    fs.close();
    return ret;
}


bool ImageIO::writePFM(
    const std::string& fileName,
    const HDRImage* const hdr)
{
    Assert(hdr != NULL);

    bool hok = writePFMHeader(fileName, hdr);
    if(!hok){
        return false;
    }
    bool rok = writePFMRaster(fileName, hdr);
    return rok;
}



