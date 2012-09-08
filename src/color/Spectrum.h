#ifndef SPECTRUM_WRAP_H
#define SPECTRUM_WRAP_H

//#include "color/ColorData.h" //Spectral reference data
//#include "color/LightData.h"

#define USE_DYNAMIC_MEM

#ifdef USE_DYNAMIC_MEM
    #include "color/DynamicSpectrum.h"
    typedef DynamicSpectrum Spectrum;
#else
    //#include "color/NoDynSpectrum.h"
    //typedef NoDynSpectrum Spectrum;
#endif


#endif //SPECTRUM_WRAP_H
