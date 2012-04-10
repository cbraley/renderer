#ifndef SPECTRUM_PLOTTER_H
#define SPECTRUM_PLOTTER_H

#include <vector>
#include <string>
//--
#include "Spectrum.h"

namespace SpectrumPlotter{

    typedef struct RGBColor{
        unsigned char R,G,B;
    }RGBColor;

    /// \brief POD Type encompassing all the data required to plot an 
    /// individual spectrum.
    typedef struct SpectrumPlotData{
    public:
        
        /// \brief Parameterized ctor taking the spectral data and the
        /// spectrum name.
        SpectrumPlotData(const Spectrum& s, const std::string& name);

        //Main components
        Spectrum data;        //Spectum to plot
        std::string specName; //Name of this data in the legend

        //Appearance 
        //all of these are set to reasonable values in the default c-tor
        int lineWidth;        //Width of line(1-5 is reasonable)
        RGBColor lineColor;   //Color of output line
        bool useDfltColor;    //Let GNUPlot choose colors by itself?
    }SpectrumPlotData;


    /// \brief POD Type encompassing the output look and global characteristics
    /// of the plot.
    typedef struct PlotStyle{
    public:
        PlotStyle(const std::string& plotTitle);

        std::string title;   //Title of whole thing
        std::string xlabel;  //Plot x label
        std::string ylabel;  //Plot y label
        bool useGrid;        //Show grid?
        int nmMin;           //Min wavelength
        int nmMax;           //Max wavelength
        float sampleRate;    //How often to sample?
    }PlotStyle;

    


    std::string getGNUPlotCommands(const std::vector<SpectrumPlotData>& toPlot,
        const PlotStyle& ps);


    bool writeGNUPlotCommandsToFile(
        const std::vector<SpectrumPlotData>& toPlot,
        const PlotStyle& ps, const std::string& fileName);


}

#endif //SPECTRUM_PLOTTER_H
