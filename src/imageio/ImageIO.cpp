#include "imageio/ImageIO.h"
//--
#include <fstream>
//--
#include "utils/Assert.h"


bool ImageIO::writePPM(const std::string& fileName, const LDRImage* const img,
    const std::string& comment)
{

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
        f << "#" << comment << std::endl;              //user's comment
    }
    f << img->width( ) << " " << std::endl << //Raster width
         img->height() << " " << std::endl << //Raster height
         maxVal               << std::endl;   //Maximum value in image
    f.close();

    //Open up a stream to write the raster data in binary
    std::fstream fRaster(fileName.c_str(),  //Need to have app for "append"
        std::fstream::out | std::fstream::binary | std::fstream::app);
    if(!fRaster){ //Make sure stream opened OK
        f.close();
        return false;
    }

    //PPM raster
    const int rows = img->height();
    //unsigned char rgb[3] = {0,0,0};
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






/* Read/write 1 and 3 channel PFM files, public domain Connelly Barnes 2007. */



#include <cstdlib>
#include <cstring>
#include <cstdio>


typedef unsigned char byte;

inline static int is_little_endian() {
  if (sizeof(float) != 4) { printf("Bad float size.\n"); exit(1); }
  byte b[4] = { 255, 0, 0, 0 };
  return *((float *) b) < 1.0;
}



//3 channel
void write_pfm_file3(const char *filename, float *depth, int w, int h) {
  FILE *f = fopen(filename, "wb");
  double scale = 1.0;
  if (is_little_endian()) { scale = -scale; }
  fprintf(f, "PF\n%d %d\n%lf\n", w, h, scale);
  int channels = 3;
  for (int i = 0; i < w*h*channels; i++) {
    float d = depth[i];
    fwrite((void *) &d, 1, 4, f);
  }
  fclose(f);
}


//Single channel
void write_pfm_file(const char *filename, float *depth, int w, int h) {
  FILE *f = fopen(filename, "wb");
  double scale = 1.0;
  if (is_little_endian()) { scale = -scale; }
  fprintf(f, "Pf\n%d %d\n%lf\n", w, h, scale);
  for (int i = 0; i < w*h; i++) {
    float d = depth[i];
    fwrite((void *) &d, 1, 4, f);
  }
  fclose(f);
}



bool ImageIO::writePFM(const std::string& fileName, const HDRImage* const img,
    const std::string& comment)
{
    HDRImage* hack = const_cast<HDRImage*>(img);

    write_pfm_file3(fileName.c_str(), (float*) (hack->getPixelBuf()), img->width(), img->height());
    return true;
}
