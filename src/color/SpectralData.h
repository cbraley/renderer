#ifndef COLOR_SPECTRAL_DATA_H
#define COLOR_SPECTRAL_DATA_H

/**
 *  Namespace containing const static arrrays of useful spectral data.
 *  Most of the data comes from a link on this page:
 *      http://www.cis.rit.edu/mcsl/online/cie.php
 *      and
 *      http://www.cvrl.org/cones.htm
 *      and
 *      http://www.cis.rit.edu/research/mcsl2/online/cie.php
 */
namespace SpectralData{

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Regularly sampled 1nm data in the range [300nm, 830nm] -----------------
    // Data includes:
    //     CIE Illuminants A and D65
    //     CIE Luminous Effeciency function (both Photopic and Scotopic)
    //     X,Y, and Z CMFs for 2 degree FOV
    //     X,Y, and Z CMFs for 10 degree FOV
    // Data from:
    //    http://www.cis.rit.edu/research/mcsl2/online/cie.php
    //    http://www.cis.rit.edu/research/mcsl2/online/CIE/all_1nm_data.xls
    //    Also stored in data/all_1nm_data.csv
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    // Array bounds for 1 nm data
    static const int CIE_1NM_NUM_SAMPLES   = 531;
    static const int CIE_1NM_NUM_DATASETS  = 11;

    //  Wavelength ranges (samples are regular)
    static const int CIE_1NM_DATA_NM_START = 300;
    static const int CIE_1NM_DATA_NM_END   = 830;
    static const int CIE_1NM_DATA_NM_STEP  = 1;

    //  The raw data array
    extern const float CIE_1NM[CIE_1NM_NUM_SAMPLES][CIE_1NM_NUM_DATASETS];

    // These indices allow us to pick which function we are sampling.
    // The below indices can be used to index into the 1nm color data CIE_1NM like so:
    //    CIE_1NM[i][CIE_1NM_DATA_INDEX_CIE_LUMINOUS_EFFICIENCY_SCOTOPIC]
    //    would give you the value of the function
    //    CIE_1NM_DATA_INDEX_CIE_LUMINOUS_EFFICIENCY_SCOTOPIC
    //    function at wavelength CIE_1NM[i][CIE_1NM_DATA_INDEX_WAVELENS], or
    //    equivalently, at wavelength
    //    CIE_1NM_DATA_NM_START + (i * CIE_1NM_DATA_NM_STEP)
    static const int CIE_1NM_DATA_INDEX_WAVELENS                              = 0;
    static const int CIE_1NM_DATA_INDEX_CIE_ILLUM_A                           = 1;
    static const int CIE_1NM_DATA_INDEX_CIE_ILLUM_D65                         = 2;
    static const int CIE_1NM_DATA_INDEX_CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1988 = 3;
    static const int CIE_1NM_DATA_INDEX_CIE_LUMINOUS_EFFICIENCY_SCOTOPIC      = 4;
    static const int CIE_1NM_DATA_INDEX_X_BAR_2_DEG                           = 5;
    static const int CIE_1NM_DATA_INDEX_Y_BAR_2_DEG                           = 6;
    static const int CIE_1NM_DATA_INDEX_Z_BAR_2_DEG                           = 7;
    static const int CIE_1NM_DATA_INDEX_X_BAR_10_DEG                          = 8;
    static const int CIE_1NM_DATA_INDEX_Y_BAR_10_DEG                          = 9;
    static const int CIE_1NM_DATA_INDEX_Z_BAR_10_DEG                          = 10;


    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Regularly sampled 5nm data in the range [380nm, 780nm] for the MacBeth
    // color checker.  This chart *looks similar* to the new xRite checker, but
    // there is no gaurentee their spectral properties have no changed(ie they
    // may be close to metameric).
    //
    // Data includes:
    //    All 24 patches on a standard color checker.
    //
    // Data from:
    //    http://www.cis.rit.edu/research/mcsl2/online/CIE/MacbethColorChecker.xls
    //    http://www.cis.rit.edu/research/mcsl2/online/cie.php
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    // Wavelength ranges for the 5nm Macbeth color chart data
    static const int MACBETH_COLOR_CHECKER_5NM_DATA_NM_START  = 380;
    static const int MACBETH_COLOR_CHECKER_5NM_DATA_NM_END    = 780;
    static const int MACBETH_COLOR_CHECKER_5NM_DATA_NM_STEP   = 5;

    // Array bounds for the Macbeth color chart 5nm data
    static const int MACBETH_COLOR_CHECKER_5NM_NUM_SAMPLES    = 81;
    static const int MACBETH_COLOR_CHECKER_5NM_NUM_SQUARES    = 25; 
    
    //Important note - The first square is square 1 *not square 0*!
    static const int MAXBETH_COLOR_CHECKER_FIRST_SQUARE_INDEX = 1;

    // Note that squares are numbered in standard Western reading order (left to right, top to bottom) starting at index 1.  
    // Square 1 is brown on the typical chart, square 17 is pink, etc.
    extern const float MACBETH_COLOR_CHECKER_5NM[MACBETH_COLOR_CHECKER_5NM_NUM_SAMPLES][MACBETH_COLOR_CHECKER_5NM_NUM_SQUARES];


    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Regularly sampled 5nm data in the range [365nm, 780nm] -----------------
    // Data includes:
    //     X,Y, and Z CMFs for 2 degree FOV
    //     X,Y, and Z CMFs for 10 degree FOV
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------


    // Wavelength range of the CIE_OBSERVER_5NM data, declared as int's since floats are
    // can't be declared "static const" due to C++ restrictions.
    static const int CIE_OBSERVER_5NM_DATA_NM_START = 365;
    static const int CIE_OBSERVER_5NM_DATA_NM_END   = 780;
    static const int CIE_OBSERVER_5NM_DATA_NM_STEP  = 5;

    // Array bounds for the CIE_OBSERVER_5NM data
    static const int CIE_OBSERVER_5NM_NUM_SAMPLES = 84;

    // Data from:
    //      http://www.cis.rit.edu/research/mcsl2/online/CIE/StdObsFuncs.htm
    // For:
    // CIE_OBSERVER_5NM[i][j]
    // i indexes over wavelength, whereas j indexes over which function you are looking at.
    //     j = 0 = Read off the wavelength in nanometers.
    //     j = 1 = X color matching function, 2 degree FOV.
    //     j = 2 = Y color matching function, 2 degree FOV.
    //     j = 3 = Z color matching function, 2 degree FOV.
    //     j = 4 = X color matching function, 10 degree FOV.
    //     j = 5 = Y color matching function, 10 degree FOV.
    //     j = 6 = Z color matching function, 10 degree FOV.
    extern const float CIE_OBSERVER_5NM[CIE_OBSERVER_5NM_NUM_SAMPLES][7];
}

#endif //COLOR_SPECTRAL_DATA_H
