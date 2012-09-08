#ifndef COLOR_DATA_H
#define COLOR_DATA_H

#include <cstring>
//--
#include "utils/Assert.h"

/**
 *  File stores the raw data for the XYZ spectral matching curves (CIE CMFs)
 *  in both 2-degree and 10 degree FOV.  Data taken from:
 *      http://www.cis.rit.edu/research/mcsl2/online/CIE/all_1nm_data.xls
 *
 *  Sampled at 1nm intervals.
 *
 *  Comes with a super simple 1-function API for accessing the data.
 *
 *  @date Oct 3, 2011
 */

enum CMF_ID{
    X_CMF_2DEG,
    Y_CMF_2DEG,
    Z_CMF_2DEG,
    X_CMF_10DEG,
    Y_CMF_10DEG,
    Z_CMF_10DEG,
    PHOTOPIC,
    SCOPTIC
};

typedef struct CMF{
    float* data;
    float nmMin;
    float nmMax;
    float nmStep;
    int N;
}CMF;

/**
 *  Get a copy of the data for a spcified curve.  Which curve is specified through the
 *  first param, an enum.
 *
 *  Note that the data array in the return CMF is dynamically allocated and the callee is responsible for
 *  deallocating it using delete[].
 */
CMF getCMF(CMF_ID curveType);



#endif //COLOR_DATA_H
