#include "SpectrumPlotter.h"
//--
#include <sstream>
#include <fstream>
#include <limits>
//--
#include "ColorUtils.h"
#include "Assert.h"

namespace SpectrumPlotter{

SpectrumPlotData::SpectrumPlotData(const Spectrum& s, const std::string& name) :
    data(s), specName(name),
    lineWidth(3), useDfltColor(true)
{}


    /// \brief POD Type encompassing the output look and global characteristics
    /// of the plot.
PlotStyle::PlotStyle(const std::string& plotTitle) :
    title(plotTitle),
    xlabel("Wavelength (nm)"), ylabel(""),
    useGrid(true), nmMin(400), nmMax(720),
    sampleRate(2.0f)
{}


std::string getGNUPlotCommands(
    const std::vector<SpectrumPlotData>& toPlot,
    const PlotStyle& ps)
{
    Assert(ps.nmMin <= ps.nmMax);
    Assert(ps.sampleRate > 0.0f);

    std::stringstream ss;

    //GNUPlot does not like scientific notation
    ss.precision(std::numeric_limits<float>::digits10);

    //Setup main look of plot
    ss << "set title \""  << ps.title  << "\"" << std::endl;
    ss << "set xlabel \"" << ps.xlabel << "\"" << std::endl;
    ss << "set ylabel \"" << ps.ylabel << "\"" << std::endl;
    if(ps.useGrid){
        ss << "set grid" << std::endl;
    }
    ss << "set xtics 50"  << std::endl;
    ss << "set mxtics 10" << std::endl;
    ss << std::endl << std::endl << std::endl;

    //Setup the draw styles
    for(size_t i = 0; i < toPlot.size(); i++){
        SpectrumPlotData p = toPlot[i];
        ss << "set style line " << i+1 << " ";
        if(!p.useDfltColor){
            ss << "lt rgb \"" << ColorUtils::getHexStr(p.lineColor.R, p.lineColor.G, p.lineColor.B,"#") << "\" ";
        }
        ss << " lw " << p.lineWidth << std::endl;
    }
    ss << std::endl << std::endl << std::endl;

    //The plot line
    ss << "plot ";
    for(size_t i = 0; i < toPlot.size(); i++){
        ss << " '-' with lines title \"" << toPlot[i].specName << "\" ls " << i+1 << " ";
        if(i != toPlot.size()-1){
            ss << ", ";
        }
    }
    ss << std::endl;

    //The raw data
    for(size_t i = 0; i < toPlot.size(); i++){
        for(float nm = ps.nmMin; nm <= ps.nmMax; nm += ps.sampleRate){
            ss << nm << " " << toPlot[i].data(nm) << std::endl;
        }
        ss << "e" << std::endl << std::endl;
    }

    ss << std::endl << std::endl << std::endl;
    return ss.str();
}


bool writeGNUPlotCommandsToFile(
    const std::vector<SpectrumPlotData>& toPlot,
    const PlotStyle& ps, const std::string& fileName)
{
    //Try to open stream
    std::fstream fs(fileName.c_str(), std::fstream::out);
    if(!fs.good()){
        fs.close();
        return false;
    }

    //Write the string to file
    fs << getGNUPlotCommands(toPlot, ps) << std::endl;

    bool wok = fs.good();
    fs.close();
    return wok;
}

}

