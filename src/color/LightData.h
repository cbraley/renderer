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

//A series
static const float A_SERIES_NM_MIN = 300.0f;
static const float A_SERIES_NM_MAX  = 780.0f;
static const float A_SERIES_NM_STEP = 5.0f;
static const int A_SERIES_N = 97;
static const float A_SERIES_DATA[97] = {
    0.930483, //300
    1.128210, //305
    1.357690,
    1.622190,
    1.925080,
    2.269800,
    2.659810,
    3.098610,
    3.589680,
    4.136480,
    4.742380,
    5.410700,
    6.144620,
    6.947200,
    7.821350,
    8.769800,
    9.795100,
    10.899600,
    12.085300,
    13.354300,
    14.708000,
    16.148000,
    17.675300,
    19.290700,
    20.995000,
    22.788300,
    24.670900,
    26.642500,
    28.702700,
    30.850800,
    33.085900,
    35.406800,
    37.812100,
    40.300200,
    42.869300,
    45.517400,
    48.242300,
    51.041800,
    53.913200,
    56.853900,
    59.861100,
    62.932000,
    66.063500,
    69.252500,
    72.495900,
    75.790300,
    79.132600,
    82.519300,
    85.947000,
    89.412400,
    92.912000,
    96.442300,
    100.000000,
    103.582000,
    107.184000,
    110.803000,
    114.436000,
    118.080000,
    121.731000,
    125.386000,
    129.043000,
    132.697000,
    136.346000,
    139.988000,
    143.618000,
    147.235000,
    150.836000,
    154.418000,
    157.979000,
    161.516000,
    165.028000,
    168.510000,
    171.963000,
    175.383000,
    178.769000,
    182.118000,
    185.429000,
    188.701000,
    191.931000,
    195.118000,
    198.261000,
    201.359000,
    204.409000,
    207.411000,
    210.365000,
    213.268000,
    216.120000,
    218.920000,
    221.667000,
    224.361000,
    227.000000,
    229.585000,
    232.115000,
    234.589000,
    237.008000,
    239.370000,
    241.675000 //780
};


//D series
static const float D65_SERIES_NM_MIN  = 300.0f;
static const float D65_SERIES_NM_MAX  = 830.0f;
static const float D65_SERIES_NM_STEP = 5.0f;
static const int D65_SERIES_N         = 107;
static const float D65_SERIES_DATA[107] = {
    0.034100,
    1.664300,
    3.294500,
    11.765200,
    20.236000,
    28.644700,
    37.053500,
    38.501100,
    39.948800,
    42.430200,
    44.911700,
    45.775000,
    46.638300,
    49.363700,
    52.089100,
    51.032300,
    49.975500,
    52.311800,
    54.648200,
    68.701500,
    82.754900,
    87.120400,
    91.486000,
    92.458900,
    93.431800,
    90.057000,
    86.682300,
    95.773600,
    104.865000,
    110.936000,
    117.008000,
    117.410000,
    117.812000,
    116.336000,
    114.861000,
    115.392000,
    115.923000,
    112.367000,
    108.811000,
    109.082000,
    109.354000,
    108.578000,
    107.802000,
    106.296000,
    104.790000,
    106.239000,
    107.689000,
    106.047000,
    104.405000,
    104.225000,
    104.046000,
    102.023000,
    100.000000,
    98.167100,
    96.334200,
    96.061100,
    95.788000,
    92.236800,
    88.685600,
    89.345900,
    90.006200,
    89.802600,
    89.599100,
    88.648900,
    87.698700,
    85.493600,
    83.288600,
    83.493900,
    83.699200,
    81.863000,
    80.026800,
    80.120700,
    80.214600,
    81.246200,
    82.277800,
    80.281000,
    78.284200,
    74.002700,
    69.721300,
    70.665200,
    71.609100,
    72.979000,
    74.349000,
    67.976500,
    61.604000,
    65.744800,
    69.885600,
    72.486300,
    75.087000,
    69.339800,
    63.592700,
    55.005400,
    46.418200,
    56.611800,
    66.805400,
    65.094100,
    63.382800,
    63.843400,
    64.304000,
    61.877900,
    59.451900,
    55.705400,
    51.959000,
    54.699800,
    57.440600,
    58.876500,
    60.312500
};



//E series (constant value illuminant)
static const float E_SERIES_NM_MIN  = 380.0f;
static const float E_SERIES_NM_MAX  = 780.0f;
static const float E_SERIES_NM_STEP = E_SERIES_NM_MAX - E_SERIES_NM_MIN;
static const int E_SERIES_N = 2;
static const float E_SERIES_DATA[2] = {100.0f , 100.0f};


//F series = [1,2,3,...,12]
static const float F_SERIES_NM_MIN  = 380.0f;
static const float F_SERIES_NM_MAX  = 780.0f;
static const float F_SERIES_NM_STEP = 5.0f;
static const int F_SERIES_N = 81;
static const float F_SERIES_DATA[81][13] = {
    {380,    1.87,    1.18,    0.82,    0.57,    1.87,    1.05,    2.56,    1.21,    0.9,    1.11,    0.91,    0.96},
    {385,    2.36,    1.48,    1.02,    0.7,    2.35,    1.31,    3.18,    1.5,    1.12,    0.63,    0.63,    0.64},
    {390,    2.94,    1.84,    1.26,    0.87,    2.92,    1.63,    3.84,    1.81,    1.36,    0.62,    0.46,    0.45},
    {395,    3.47,    2.15,    1.44,    0.98,    3.45,    1.9,    4.53,    2.13,    1.6,    0.57,    0.37,    0.33},
    {400,    5.17,    3.44,    2.57,    2.01,    5.1,    3.11,    6.15,    3.17,    2.59,    1.48,    1.29,    1.19},
    {405,    19.49,    15.69,    14.36,    13.75,    18.91,    14.8,    19.37,    13.08,    12.8,    12.16,    12.68,    12.48},
    {410,    6.13,    3.85,    2.7,    1.95,    6,    3.43,    7.37,    3.83,    3.05,    2.12,    1.59,    1.12},
    {415,    6.24,    3.74,    2.45,    1.59,    6.11,    3.3,    7.05,    3.45,    2.56,    2.7,    1.79,    0.94},
    {420,    7.01,    4.19,    2.73,    1.76,    6.85,    3.68,    7.71,    3.86,    2.86,    3.74,    2.46,    1.08},
    {425,    7.79,    4.62,    3,    1.93,    7.58,    4.07,    8.41,    4.42,    3.3,    5.14,    3.33,    1.37},
    {430,    8.56,    5.06,    3.28,    2.1,    8.31,    4.45,    9.15,    5.09,    3.82,    6.75,    4.49,    1.78},
    {435,    43.67,    34.98,    31.85,    30.28,    40.76,    32.61,    44.14,    34.1,    32.62,    34.39,    33.94,    29.05},
    {440,    16.94,    11.81,    9.47,    8.03,    16.06,    10.74,    17.52,    12.42,    10.77,    14.86,    12.13,    7.9},
    {445,    10.72,    6.27,    4.02,    2.55,    10.32,    5.48,    11.35,    7.68,    5.84,    10.4,    6.95,    2.65},
    {450,    11.35,    6.63,    4.25,    2.7,    10.91,    5.78,    12,    8.6,    6.57,    10.76,    7.19,    2.71},
    {455,    11.89,    6.93,    4.44,    2.82,    11.4,    6.03,    12.58,    9.46,    7.25,    10.67,    7.12,    2.65},
    {460,    12.37,    7.19,    4.59,    2.91,    11.83,    6.25,    13.08,    10.24,    7.86,    10.11,    6.72,    2.49},
    {465,    12.75,    7.4,    4.72,    2.99,    12.17,    6.41,    13.45,    10.84,    8.35,    9.27,    6.13,    2.33},
    {470,    13,    7.54,    4.8,    3.04,    12.4,    6.52,    13.71,    11.33,    8.75,    8.29,    5.46,    2.1},
    {475,    13.15,    7.62,    4.86,    3.08,    12.54,    6.58,    13.88,    11.71,    9.06,    7.29,    4.79,    1.91},
    {480,    13.23,    7.65,    4.87,    3.09,    12.58,    6.59,    13.95,    11.98,    9.31,    7.91,    5.66,    3.01},
    {485,    13.17,    7.62,    4.85,    3.09,    12.52,    6.56,    13.93,    12.17,    9.48,    16.64,    14.29,    10.83},
    {490,    13.13,    7.62,    4.88,    3.14,    12.47,    6.56,    13.82,    12.28,    9.61,    16.73,    14.96,    11.88},
    {495,    12.85,    7.45,    4.77,    3.06,    12.2,    6.42,    13.64,    12.32,    9.68,    10.44,    8.97,    6.88},
    {500,    12.52,    7.28,    4.67,    3,    11.89,    6.28,    13.43,    12.35,    9.74,    5.94,    4.72,    3.43},
    {505,    12.2,    7.15,    4.62,    2.98,    11.61,    6.2,    13.25,    12.44,    9.88,    3.34,    2.33,    1.49},
    {510,    11.83,    7.05,    4.62,    3.01,    11.33,    6.19,    13.08,    12.55,    10.04,    2.35,    1.47,    0.92},
    {515,    11.5,    7.04,    4.73,    3.14,    11.1,    6.3,    12.93,    12.68,    10.26,    1.88,    1.1,    0.71},
    {520,    11.22,    7.16,    4.99,    3.41,    10.96,    6.6,    12.78,    12.77,    10.48,    1.59,    0.89,    0.6},
    {525,    11.05,    7.47,    5.48,    3.9,    10.97,    7.12,    12.6,    12.72,    10.63,    1.47,    0.83,    0.63},
    {530,    11.03,    8.04,    6.25,    4.69,    11.16,    7.94,    12.44,    12.6,    10.76,    1.8,    1.18,    1.1},
    {535,    11.18,    8.88,    7.34,    5.81,    11.54,    9.07,    12.33,    12.43,    10.96,    5.71,    4.9,    4.56},
    {540,    11.53,    10.01,    8.78,    7.32,    12.12,    10.49,    12.26,    12.22,    11.18,    40.98,    39.59,    34.4},
    {545,    27.74,    24.88,    23.82,    22.59,    27.78,    25.22,    29.52,    28.96,    27.71,    73.69,    72.84,    65.4},
    {550,    17.05,    16.64,    16.14,    15.11,    17.73,    17.46,    17.05,    16.51,    16.29,    33.61,    32.61,    29.48},
    {555,    13.55,    14.59,    14.59,    13.88,    14.47,    15.63,    12.44,    11.79,    12.28,    8.24,    7.52,    7.16},
    {560,    14.33,    16.16,    16.63,    16.33,    15.2,    17.22,    12.58,    11.76,    12.74,    3.38,    2.83,    3.08},
    {565,    15.01,    17.56,    18.49,    18.68,    15.77,    18.53,    12.72,    11.77,    13.21,    2.47,    1.96,    2.47},
    {570,    15.52,    18.62,    19.95,    20.64,    16.1,    19.43,    12.83,    11.84,    13.65,    2.14,    1.67,    2.27},
    {575,    18.29,    21.47,    23.11,    24.28,    18.54,    21.97,    15.46,    14.61,    16.57,    4.86,    4.43,    5.09},
    {580,    19.55,    22.79,    24.69,    26.26,    19.5,    23.01,    16.75,    16.11,    18.14,    11.45,    11.28,    11.96},
    {585,    15.48,    19.29,    21.41,    23.28,    15.39,    19.41,    12.83,    12.34,    14.55,    14.79,    14.76,    15.32},
    {590,    14.91,    18.66,    20.85,    22.94,    14.64,    18.56,    12.67,    12.53,    14.65,    12.16,    12.73,    14.27},
    {595,    14.15,    17.73,    19.93,    22.14,    13.72,    17.42,    12.45,    12.72,    14.66,    8.97,    9.74,    11.86},
    {600,    13.22,    16.54,    18.67,    20.91,    12.69,    16.09,    12.19,    12.92,    14.61,    6.52,    7.33,    9.28},
    {605,    12.19,    15.21,    17.22,    19.43,    11.57,    14.64,    11.89,    13.12,    14.5,    8.81,    9.72,    12.31},
    {610,    11.12,    13.8,    15.65,    17.74,    10.45,    13.15,    11.6,    13.34,    14.39,    44.12,    55.27,    68.53},
    {615,    10.03,    12.36,    14.04,    16,    9.35,    11.68,    11.35,    13.61,    14.4,    34.55,    42.58,    53.02},
    {620,    8.95,    10.95,    12.45,    14.42,    8.29,    10.25,    11.12,    13.87,    14.47,    12.09,    13.18,    14.67},
    {625,    7.96,    9.65,    10.95,    12.56,    7.32,    8.96,    10.95,    14.07,    14.62,    12.15,    13.16,    14.38},
    {630,    7.02,    8.4,    9.51,    10.93,    6.41,    7.74,    10.76,    14.2,    14.72,    10.52,    12.26,    14.71},
    {635,    6.2,    7.32,    8.27,    9.52,    5.63,    6.69,    10.42,    14.16,    14.55,    4.43,    5.11,    6.46},
    {640,    5.42,    6.31,    7.11,    8.18,    4.9,    5.71,    10.11,    14.13,    14.4,    1.95,    2.07,    2.57},
    {645,    4.73,    5.43,    6.09,    7.01,    4.26,    4.87,    10.04,    14.34,    14.58,    2.19,    2.34,    2.75},
    {650,    4.15,    4.68,    5.22,    6,    3.72,    4.16,    10.02,    14.5,    14.88,    3.19,    3.58,    4.18},
    {655,    3.64,    4.02,    4.45,    5.11,    3.25,    3.55,    10.11,    14.46,    15.51,    2.77,    3.01,    3.44},
    {660,    3.2,    3.45,    3.8,    4.36,    2.83,    3.02,    9.87,    14,    15.47,    2.29,    2.48,    2.81},
    {665,    2.81,    2.96,    3.23,    3.69,    2.49,    2.57,    8.65,    12.58,    13.2,    2,    2.14,    2.42},
    {670,    2.47,    2.55,    2.75,    3.13,    2.19,    2.2,    7.27,    10.99,    10.57,    1.52,    1.54,    1.64},
    {675,    2.18,    2.19,    2.33,    2.64,    1.93,    1.87,    6.44,    9.98,    9.18,    1.35,    1.33,    1.36},
    {680,    1.93,    1.89,    1.99,    2.24,    1.71,    1.6,    5.83,    9.22,    8.25,    1.47,    1.46,    1.49},
    {685,    1.72,    1.64,    1.7,    1.91,    1.52,    1.37,    5.41,    8.62,    7.57,    1.79,    1.94,    2.14},
    {690,    1.67,    1.53,    1.55,    1.7,    1.43,    1.29,    5.04,    8.07,    7.03,    1.74,    2,    2.34},
    {695,    1.43,    1.27,    1.27,    1.39,    1.26,    1.05,    4.57,    7.39,    6.35,    1.02,    1.2,    1.42},
    {700,    1.29,    1.1,    1.09,    1.18,    1.13,    0.91,    4.12,    6.71,    5.72,    1.14,    1.35,    1.61},
    {705,    1.19,    0.99,    0.96,    1.03,    1.05,    0.81,    3.77,    6.16,    5.25,    3.32,    4.1,    5.04},
    {710,    1.08,    0.88,    0.83,    0.88,    0.96,    0.71,    3.46,    5.63,    4.8,    4.49,    5.58,    6.98},
    {715,    0.96,    0.76,    0.71,    0.74,    0.85,    0.61,    3.08,    5.03,    4.29,    2.05,    2.51,    3.19},
    {720,    0.88,    0.68,    0.62,    0.64,    0.78,    0.54,    2.73,    4.46,    3.8,    0.49,    0.57,    0.71},
    {725,    0.81,    0.61,    0.54,    0.54,    0.72,    0.48,    2.47,    4.02,    3.43,    0.24,    0.27,    0.3},
    {730,    0.77,    0.56,    0.49,    0.49,    0.68,    0.44,    2.25,    3.66,    3.12,    0.21,    0.23,    0.26},
    {735,    0.75,    0.54,    0.46,    0.46,    0.67,    0.43,    2.06,    3.36,    2.86,    0.21,    0.21,    0.23},
    {740,    0.73,    0.51,    0.43,    0.42,    0.65,    0.4,    1.9,    3.09,    2.64,    0.24,    0.24,    0.28},
    {745,    0.68,    0.47,    0.39,    0.37,    0.61,    0.37,    1.75,    2.85,    2.43,    0.24,    0.24,    0.28},
    {750,    0.69,    0.47,    0.39,    0.37,    0.62,    0.38,    1.62,    2.65,    2.26,    0.21,    0.2,    0.21},
    {755,    0.64,    0.43,    0.35,    0.33,    0.59,    0.35,    1.54,    2.51,    2.14,    0.17,    0.24,    0.17},
    {760,    0.68,    0.46,    0.38,    0.35,    0.62,    0.39,    1.45,    2.37,    2.02,    0.21,    0.32,    0.21},
    {765,    0.69,    0.47,    0.39,    0.36,    0.64,    0.41,    1.32,    2.15,    1.83,    0.22,    0.26,    0.19},
    {770,    0.61,    0.4,    0.33,    0.31,    0.55,    0.33,    1.17,    1.89,    1.61,    0.17,    0.16,    0.15},
    {775,    0.52,    0.33,    0.28,    0.26,    0.47,    0.26,    0.99,    1.61,    1.38,    0.12,    0.12,    0.1},
    {780,    0.43,    0.27,    0.21,    0.19,    0.4,    0.21,    0.81,    1.32,    1.12,    0.09,    0.09,    0.05}
};


Illuminant getIlluminant(ILLUMINANT_SERIES lamp, int index){
    Illuminant l;

    if(lamp == A){
        l.N = A_SERIES_N;
        l.nmMin = A_SERIES_NM_MIN;
        l.nmMax = A_SERIES_NM_MAX;
        l.nmStep = A_SERIES_NM_STEP;
        l.data = new float[l.N];
        for(int i = 0; i < l.N; i++){
            l.data[i] = A_SERIES_DATA[i];
        }
        return l;
    }else if(lamp == E){ //E (Constant) illuminant
        l.N = E_SERIES_N;
        l.nmMin = E_SERIES_NM_MIN;
        l.nmMax = E_SERIES_NM_MAX;
        l.nmStep = E_SERIES_NM_STEP;
        l.data = new float[l.N];
        for(int i = 0; i < l.N; i++){
            l.data[i] = E_SERIES_DATA[i];
        }
        return l;
    }else if(lamp == D){ //D series illuminant
        if(index == 65){
            l.N = D65_SERIES_N;
            l.nmMin = D65_SERIES_NM_MIN;
            l.nmMax = D65_SERIES_NM_MAX;
            l.nmStep = D65_SERIES_NM_STEP;
            l.data = new float[l.N];
            for(int i = 0; i < l.N; i++){
                l.data[i] = D65_SERIES_DATA[i];
            }
        }else{
            Assert(false); //Bad index
            l.data = NULL;
            l.N = -1;
            l.nmMax = l.nmMin = -999;
        }
        return l;
    }else if(lamp == F){ //Flourescent series
        Assert(index >= 1 && index <= 12);
        l.N      = F_SERIES_N;
        l.nmMin  = F_SERIES_NM_MIN;
        l.nmMax  = F_SERIES_NM_MAX;
        l.nmStep = F_SERIES_NM_STEP;
        l.data = new float[l.N];
        for(int i = 0; i < l.N; i++){
            l.data[i] = F_SERIES_DATA[i][index];
        }
        return l;
    }else{
        Assert(false);
        l.data = NULL;
    }
    return l;
}


#endif //LIGHT_DATA_H
