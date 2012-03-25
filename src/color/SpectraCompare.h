#ifndef SPECTRA_COMPARE_H
#define SPECTRA_COMPARE_H

#include "Spectrum.h"

/**
 *  This namespace contains functions for comparing spectra.
 *  All functions take two spectra as arguments, and return a
 *  scalar in the range [0,1] where 0 indicates total inequality
 *  and 1 indicates perfect agreement..
 *
 *  @date Oct 20, 2011
 */
namespace SpectraCompare{

//TODO: Include a weighting spectra!

/// \brief root means squared comparison.
float specCompareRMS(const Spectrum* a, const Spectrum* b);

/// \brief goodness-fit coefficient comparison.
float specCompareGFC(const Spectrum* a, const Spectrum* b);

}

#endif //SPECTRA_COMPARE_H
