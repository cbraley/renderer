#ifndef LIGHT_DATA_H
#define LIGHT_DATA_H

#include "utils/Assert.h"

/**
 * Tabulated data for various reference light sources.
 *
 * Data sources:
 *     CIE A Data:   http://www.cie.co.at/publ/abst/datatables15_2004/CIE_sel_colorimetric_tables.xls
 *     CIE D65 Data: http://www.cie.co.at/publ/abst/datatables15_2004/CIE_sel_colorimetric_tables.xls
 *     CIE C Data:
 *     CIE F Data:   http://www.cis.rit.edu/research/mcsl2/online/CIE/Fluorescents.htm
  *
 *
 */

enum ILLUMINANT_SERIES{
    A,D,E,F
};

typedef struct Illuminant{
    float* data;
    float nmMin;
    float nmMax;
    float nmStep;
    int N;

    //TODO(cbraley): Should I make this free mem?
    virtual ~Illuminant(){}
}Illuminant;

Illuminant getIlluminant(ILLUMINANT_SERIES lamp, int index);


#endif //LIGHT_DATA_H
