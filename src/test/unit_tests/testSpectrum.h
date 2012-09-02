#ifndef TEST_SPECTRUM_H
#define TEST_SPECTRUM_H

#include <UnitTest++.h>
//--
#include "color/Spectrum.h"
#include "color/SpectrumPlotter.h"
#include "math/Point.h"
//--
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace SpectrumPlotter;

SUITE(SpectrumTests){

    static const float EPS = 1e-6;

    TEST(SpectrumPlot){
        Spectrum lum_eff(Spectrum::CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951);
        Spectrum cie_x_10(Spectrum::CIE_X_10_DEG);
        Spectrum mult = lum_eff * cie_x_10;
        Spectrum add = lum_eff + cie_x_10;

        SpectrumPlotter::SpectrumPlotData ploter(mult, "lum_eff_1951");
        
        SpectrumPlotter::PlotStyle ps("lum_eff_1951_prime");
        std::vector<SpectrumPlotData> vec;
        vec.push_back(ploter);
        bool ok = SpectrumPlotter::writeGNUPlotCommandsToFile(
            vec, ps, "plots.gnuplot");
        CHECK(ok);
    }

    /*
    TEST(MakeSpecrumPlots){

        PlotStyle ps("CIE Color Matching Functions");
        ps.nmMin = 380;
        ps.nmMax = 780;

        std::vector<SpectrumPlotData> pd;
        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_X_2_DEG),
            "X CMF - 2 Degree"));
        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_Y_2_DEG),
            "Y CMF - 2 Degree"));
        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_Z_2_DEG),
            "Z CMF - 2 Degree"));

        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_X_10_DEG),
            "X CMF - 10 Degree"));
        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_Y_10_DEG),
            "Y CMF - 10 Degree"));
        pd.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_Z_10_DEG),
            "Z CMF - 10 Degree"));
    
        bool ok = writeGNUPlotCommandsToFile(pd,ps,
            "test/tmp/CMFs.gnuplot");
        CHECK(ok);

        //Lum eff functions
        std::vector<SpectrumPlotData> pd2;
        pd2.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_LUMINOUS_EFFICIENCY_SCOPTIC_1951),
            "Scotopic Luminous Efficacy "));
        pd2.push_back(SpectrumPlotData(
            Spectrum(Spectrum::CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951),
            "Photopic Luminous Efficacy "));
        ps.title = ("CIE Luminous Efficacy Functions(Judd 1951 Data)");
        ps.nmMin = 360;
        ps.nmMax = 830;
        ok = writeGNUPlotCommandsToFile(pd2,ps,
            "test/tmp/LumEff.gnuplot");
        CHECK(ok);

        //Blackbodies
        const float NM_S = 360.0f;
        const float NM_E = 830.0f;
        std::vector<SpectrumPlotData> blackbodies;
        const int KELVIN_STEP  = 500;
        const int KELVIN_START = 2500;
        const int KELVIN_END   = 5500;
        int cnt = 0;
        for(int K = KELVIN_START; K <= KELVIN_END; K += KELVIN_STEP){

            //Add spectrum
            std::stringstream s;
            s << K << " Kelvin Blackbody";
            blackbodies.push_back(SpectrumPlotData(
                Spectrum((float)K, 1.0f, 1300.0f, 2000),
                s.str()));

            //Set it to a color
            unsigned char R,G,B;
            float r,g,b; r = g = b = 0.0f;
            blackbodies[cnt].data.toRGB(r,g,b,1.0f);
            float maxVal = std::max<float>(std::max<float>(r,g),b);
            R = (unsigned char) std::max<float>((r/maxVal)*255.0f,0.0f);
            G = (unsigned char) std::max<float>((g/maxVal)*255.0f,0.0f);
            B = (unsigned char) std::max<float>((b/maxVal)*255.0f,0.0f);
            blackbodies[cnt].lineColor.R = R;
            blackbodies[cnt].lineColor.G = G;
            blackbodies[cnt].lineColor.B = B;
            //blackbodies[cnt].useDfltColor = false;

            ++cnt;

        }
        ps.nmMin = NM_S;
        ps.nmMax = NM_E;
        ps.title = ("Blackbody radiation spectra calculated with Planck's Law");
        ok = writeGNUPlotCommandsToFile(blackbodies,ps,
            "test/tmp/blackbodies_1.gnuplot");
        CHECK(ok);

        ps.nmMin = 1.0f;
        ps.nmMax = 1300.0f;
        ps.title = ("Blackbody radiation spectra calculated with Planck's Law");
        ok = writeGNUPlotCommandsToFile(blackbodies,ps,
            "test/tmp/blackbodies_2.gnuplot");
        CHECK(ok);
    }
    */


    TEST(SpectrumCopy){
        std::vector<float> vals;
        for(int i = 0; i < 10; i++){
            vals.push_back(float(i+1));
        }
        Spectrum ss(vals, 400.0f, 10.0f);
        Spectrum ss2 = Spectrum(Spectrum::CIE_Y_2_DEG);

        Spectrum ss3 = ss2 = ss; //Test for proper chaining
        CHECK_EQUAL(ss, ss2);
        CHECK_EQUAL(ss, ss3);

        Spectrum ss4(ss);
        CHECK_EQUAL(ss, ss4);
    }

    /// Test interpolation on EXTREMELY simple spectra.
    TEST(SpectrumInterpSimple){
        //Make a dirac delta spectrum
        std::vector<float> a;
        a.push_back(33.10f);
        Spectrum aS(a,400,10);

        CHECK(aS(400.0f) == 33.10f);
        CHECK(aS(500.0f) != 33.10f);
        CHECK(aS(401.0f) != 33.10f);
        CHECK(aS(399.0f) != 33.10f);
        CHECK(aS(410.0f) != 33.10f);

        //Now make a 2 item spectrum that is ascending
        std::vector<float> b;
        b.push_back(10.0f);
        b.push_back(20.0f);
        Spectrum bS(b,400,100); //400 = 10, 500 = 20
        float t = bS(400.0f);
        CHECK_CLOSE(10.0f, t, EPS);
        CHECK_CLOSE(20.0f, bS(500.0), EPS);
        CHECK_CLOSE(15.0f, bS(450.0), EPS);
        CHECK_CLOSE(12.5f, bS(425.0), EPS);
        CHECK_CLOSE(17.5f, bS(475.0), EPS);
        CHECK_EQUAL(0.0f, bS(399.9f));
        CHECK_EQUAL(0.0f, bS(501.f));
        CHECK_EQUAL(0.0f, bS(500.1f));

        //Now make a 2 item descending spectrum
        std::vector<float> c;
        c.push_back(20.0f);
        c.push_back(10.0f);
        Spectrum cS(c,400,100); //400 = 10, 500 = 20
        CHECK_CLOSE(20.0f, cS(400.0), EPS);
        CHECK_CLOSE(10.0f, cS(500.0), EPS);
        CHECK_CLOSE(15.0f, cS(450.0), EPS);
        CHECK_CLOSE(17.5f, cS(425.0), EPS);
        CHECK_CLOSE(12.5f, cS(475.0), EPS);
        CHECK_EQUAL(0.0f, cS(399.9f));
        CHECK_EQUAL(0.0f, cS(501.f));
        CHECK_EQUAL(0.0f, cS(500.1f));
    }

    //Make a spectrum with 11 samples, make sure things go OK
    TEST(SpectrumInterpMedium){
        std::vector<float> a;
        for(int i = 0; i < 11; i++){
            if(i % 2 == 0){
                a.push_back(float(i));
            }else{
                a.push_back(float(11 - i));
            }
        }
        Spectrum S(a,400.0f,10.0f);

        /*
        s[400] = 0
        s[410] = 10
        s[420] = 2
        s[430] = 8
        s[440] = 4
        s[450] = 6
        s[460] = 6
        s[470] = 4
        s[480] = 8
        s[490] = 2
        s[500] = 10
        */

        //Test all sample points
        float nm = 400.0f;
        for(int i = 0; i < 11; i++){
            float v = 0.0f;
            if(i % 2 == 0){
                v = float(i);
            }else{
                v = (float(11 - i));
            }
            CHECK_CLOSE(v, S(nm), EPS);
            nm += 10.0f;
        }

        //A few random interp tests
        CHECK_CLOSE(5.0f, S(405.0f), EPS);
        CHECK_CLOSE(6.0f, S(415.0f), EPS);
        CHECK_CLOSE(6.0f, S(495.0f), EPS);
        CHECK_CLOSE(5.0f, S(485.0f), EPS);
        CHECK_CLOSE(6.0f, S(451.0f), EPS);
        CHECK_CLOSE(6.0f, S(451.0f), EPS);
        CHECK_CLOSE(6.0f, S(455.0f), EPS);
        CHECK_CLOSE(6.0f, S(457.0f), EPS);

    }

    TEST(ToRGB){
        float r,g,b;
        r=g=b=0.0f;
        std::vector<float> a;
        a.push_back(33.10f);
        Spectrum S(a,400,10);
        S.toRGB(r,g,b,1.0f);

    }

    TEST(SpectrumTabulatedData){

        //Color matching functions
        Spectrum x2(Spectrum::CIE_X_2_DEG);
        Spectrum y2(Spectrum::CIE_Y_2_DEG);
        Spectrum z2(Spectrum::CIE_Z_2_DEG);
        //--
        Spectrum x10(Spectrum::CIE_X_10_DEG);
        Spectrum y10(Spectrum::CIE_Y_10_DEG);
        Spectrum z10(Spectrum::CIE_Z_10_DEG);
        //--
        Spectrum v(Spectrum::CIE_LUMINOUS_EFFICIENCY_PHOTOPIC_1951);
        Spectrum vP(Spectrum::CIE_LUMINOUS_EFFICIENCY_SCOPTIC_1951);


        //Flourescents
        Spectrum f1(Spectrum::CIE_ILLUM_F_1);
        Spectrum f2(Spectrum::CIE_ILLUM_F_2);
        Spectrum f3(Spectrum::CIE_ILLUM_F_3);
        Spectrum f4(Spectrum::CIE_ILLUM_F_4);
        Spectrum f5(Spectrum::CIE_ILLUM_F_5);
        Spectrum f6(Spectrum::CIE_ILLUM_F_6);
        Spectrum f7(Spectrum::CIE_ILLUM_F_7);
        Spectrum f8(Spectrum::CIE_ILLUM_F_8);
        Spectrum f9(Spectrum::CIE_ILLUM_F_9);
        Spectrum f10(Spectrum::CIE_ILLUM_F_10);
        Spectrum f11(Spectrum::CIE_ILLUM_F_11);
        Spectrum f12(Spectrum::CIE_ILLUM_F_12);

        //Standard illuminants
        Spectrum A(Spectrum::CIE_ILLUM_A);
        Spectrum D65(Spectrum::CIE_ILLUM_D_65);
        Spectrum E(Spectrum::CIE_ILLUM_E);

        /*
        //Make sure each spectrum is at least constructable
        //Human vis 2nm
        Spectrum sx2 = Spectrum(Spectrum::CIE_X_2_DEG);
        std::cout << "point B" << std::endl;
        Spectrum sy2 = Spectrum(Spectrum::CIE_Y_2_DEG);
        Spectrum sz2 = Spectrum(Spectrum::CIE_Z_2_DEG);
        //Human vis 10nm
        Spectrum sx10 = Spectrum(Spectrum::CIE_X_10_DEG);
        Spectrum sy10 = Spectrum(Spectrum::CIE_Y_10_DEG);
        Spectrum sz10 = Spectrum(Spectrum::CIE_Z_10_DEG);

        //std illuminants
        Spectrum stdA = Spectrum(Spectrum::CIE_ILLUM_A);
        Spectrum stdB = Spectrum(Spectrum::CIE_ILLUM_B);
        */
    }


    TEST(SpectrumBlackBody){
        //Construct a few black body radioators, larger temp
        //should mean more area under the curve
        const float MIN_BB_K = 3000.0f;
        const float MAX_BB_K = 8000.0f;
        const float BB_K_STEP = 100.0f;
        const float BB_MIN_NM = 300.0f;
        const float BB_MAX_NM = 1000.0f;
        const int BB_N = 500;
        Spectrum prev(MIN_BB_K, BB_MIN_NM, BB_MAX_NM, BB_N);
        for(float kelvin = MIN_BB_K + BB_K_STEP; kelvin < MAX_BB_K;
            kelvin += BB_K_STEP)
        {
            Spectrum curr(kelvin, BB_MIN_NM, BB_MAX_NM, BB_N);
            CHECK(curr.defIntegralTrapezoid(BB_MIN_NM, BB_MAX_NM) >
                prev.defIntegralTrapezoid(BB_MIN_NM, BB_MAX_NM));
            prev = curr;
        }
    }

    //Test spectral interpolation on a hard, densely sampled spectrum
    inline float sampleMe(float nm){
        return fabsf(2000.0f * sinf(nm / 20.0f) / (nm*nm)  );
    }
    TEST(SpectrumInterpHard){
        //Change these constants to modify the params of the test
        const int NUM_SAMPS  = 1000; //Samples in the spectrum
        const float NM_START = 400.0f;
        const float NM_END   = 720.0f;
        const float STEP_TEST = .1f;
        const float ERR_MAX   = 1e-4;


        float* data = NULL;
        data = new float[NUM_SAMPS];
        float nm = 400.0f;
        const float step = (NM_END - NM_START) / float(NUM_SAMPS-1);

        for(int i = 0; i < NUM_SAMPS; i++){
            data[i] = sampleMe(nm);
            nm += step;
        }
        Spectrum tough(data, NM_START, NM_END, step, NUM_SAMPS);
        delete[] data;

        //Go for it!
        for(float nmTest = NM_START; nmTest < NM_END; nmTest += STEP_TEST){
            const float expected = sampleMe(nmTest);
            const float actual   = tough(nmTest);
            const float err      = fabsf(expected - actual);
            CHECK(err < ERR_MAX);
        }

        //Now check some OOB spots
        CHECK_EQUAL(0.0f, tough(399.9f));
        CHECK_EQUAL(0.0f, tough(720.1f));
    }


    TEST(SpectrumIntegrateSimple){

        //Here we just point sample a linear function that
        //goes from 0 to 10 over [500,600]nm
        //...the trapezoid rule should get the exact solution here
        std::vector<float> vals;
        const int N = 10;
        for(int i = 0; i <= N; i++){
            vals.push_back((float)i);
        }

        Spectrum s(vals, 500.0f, 10.0f);

        //OOB stuff should integrate to 0
        CHECK_CLOSE(0.0f, s.defIntegralTrapezoid(400.0f, 499.0f), EPS);
        CHECK_CLOSE(0.0f, s.defIntegralTrapezoid(601.0f, 1000.0f), EPS);

        //Easy tests that won't invoke edge cases
        CHECK_CLOSE(100.0f * 10.0f * 0.5f, s.defIntegralTrapezoid(400.0f, 700.0f), EPS);
        CHECK_CLOSE(100.0f * 10.0f * 0.5f, s.defIntegralTrapezoid(500.0f, 600.0f), EPS);
        CHECK_CLOSE(50.0f * 7.5f, s.defIntegralTrapezoid(550.0f, 600.0f), EPS);
        CHECK_CLOSE(50.0f * 2.5f, s.defIntegralTrapezoid(500.0f, 550.0f), EPS);

        //Edge cases!!!
        CHECK_CLOSE(10.0f, s.defIntegralTrapezoid(505.0f, 515.0f), EPS);
        CHECK_CLOSE(5.0f * 9.75f, s.defIntegralTrapezoid(595.0f, 715.0f), EPS);

        //Constant spectra tests
        for(int k = 0; k < 30; k++){
            float step = 100.0f * float(k+1);
            float constants[5] = {0.0f, 1.0f, 2.0f, 44.0f, 10.0f };
            for(int i = 0; i < 5; i++){

                std::vector<float> temp;
                for(int j = 0; j < 100; j++){
                    temp.push_back(constants[i]);
                }

                Spectrum c(temp, 0.0f, step);


                //Value = (width of interval) * consant value
                float a,b;
                a = 300.0f; b = 400.0f;
                CHECK_CLOSE((b-a)*constants[i], c.defIntegralTrapezoid(a,b), EPS);

                a = 320.0f; b = 420.0f;
                CHECK_CLOSE((b-a)*constants[i], c.defIntegralTrapezoid(a,b), EPS);

                a = 300.0f; b = 700.0f;
                CHECK_CLOSE((b-a)*constants[i], c.defIntegralTrapezoid(a,b), EPS);

                a = 311.0f; b = 410.0f;
                CHECK_CLOSE((b-a)*constants[i], c.defIntegralTrapezoid(a,b), EPS);

                a = 404.0f; b = 405.0f;
                CHECK_CLOSE((b-a)*constants[i], c.defIntegralTrapezoid(a,b), EPS);
            }
        }
    }


    /*
    TEST(DiskIO){
        const float BB_MIN_NM = 300.0f;
        const float BB_MAX_NM = 1000.0f;
        const int BB_N = 10;
        float kelvin = 300.0f;
        Spectrum bb(kelvin, BB_MIN_NM, BB_MAX_NM, BB_N);

        const std::string SPATH("test/tmp/bb.spectrum");
        bool ok = bb.toDisk(SPATH);
        CHECK(ok);

        bool readOK = false;
        Spectrum bbcopy = Spectrum::fromDisk(SPATH, readOK);
        CHECK(readOK);

        CHECK_EQUAL(bb, bbcopy);
    }
    */

    TEST(SpectrumToXYZ){
        Spectrum y2(Spectrum::CIE_Y_2_DEG);
    }


    /*
    TEST(SpectrumBlack){

    }

    TEST(SpectrumToXYZ){

    }

    TEST(SpectrumToRGB){
        //TODO: Make better tests
        //These tests just check roughly what the output should be

        std::vector<float> nms, vals;
        float nmAccum = 400.0f;
        float gap = 50.0f;
        unsigned int N = 7;
        for(unsigned int i = 0; i < N; i++){
            nms.push_back(nmAccum);
            vals.push_back(0.0f);
            nmAccum += gap;
        }

        const float V = 10.0f;

        std::vector<float> reddish  = vals;
        reddish[N-1] = V;
        reddish[N-2] = V;

        std::vector<float> greenish = vals;
        greenish[2] = V;
        greenish[3] = V;

        std::vector<float> blueish  = vals;
        blueish[0] = V;
        blueish[1] = V;


        std::vector<float> blackish = vals; //all 0's

        std::vector<float> whiteish = vals; //All V's
        for(unsigned int i = 0; i < N; i++){
            whiteish[i] = V;
        }


        //--
        unsigned int sNum = nms.size();
        Spectrum red  (sNum, reddish , nms);
        Spectrum green(sNum, greenish, nms);
        Spectrum blue (sNum, blueish , nms);
        Spectrum white(sNum, whiteish, nms);
        Spectrum black(sNum, blackish, nms);

        float redR, redG, redB, greenR, greenG, greenB, blueR, blueG, blueB,
            whiteR, whiteG, whiteB, blackR, blackG, blackB;
        red.getRGB(redR, redG, redB);
        green.getRGB(greenR, greenG, greenB);
        blue.getRGB(blueR, blueG, blueB);
        white.getRGB(whiteR, whiteG, whiteB);
        black.getRGB(blackR, blackG, blackB);

#define PRINT_SPECTRA (1)
#ifdef PRINT_SPECTRA
        std::cout << "red   " << red   << std::endl;
        std::cout << "RGB = " << Point(redR, redG, redB) << std::endl;

        std::cout << "green " << green << std::endl;
        std::cout << "RGB = " << Point(greenR, greenG, greenB) << std::endl;

        std::cout << "blue  " << blue  << std::endl;
        std::cout << "RGB = " << Point(blueR, blueG, blueB) << std::endl;

        std::cout << "white " << white << std::endl;
        std::cout << "RGB = " << Point(whiteR, whiteG, whiteB) << std::endl;

        std::cout << "black " << black << std::endl;
        std::cout << "RGB = " << Point(blackR, blackG, blackB) << std::endl;
#endif
#undef PRINT_SPECTRA

        CHECK(redR > redG && redR > redB);
        CHECK(greenG > greenR && greenG > greenB);
        CHECK(blueB > blueR && blueB > blueG);
        CHECK(whiteR == whiteG && whiteR == whiteB);
        CHECK(blackR == blackG && blackR == blackB && blackB == 0.0f);
        CHECK(black.isBlack());


    }
    */

    /*
    TEST(SpectrumInteractive){
        std::cout << "Enter nm values followed by spectral vals" << std::endl;
        std::cout << "If you want to stop use both as -999." << std::endl;
        float nm, v;
        nm = v = 0.0f;
        std::vector<float> vals, nms;
        while(true){
            std::cin >> nm;
            std::cin >> v;
            if(nm == -999.0f && v == -999.0f){
                break;
            }else{
                nms.push_back(nm);
                vals.push_back(v);
            }
        }
        Spectrum I(nms.size(), vals, nms);
        std::cout << std::endl << I << std::endl;
        std::cout << "Spectrum is black = " << I.isBlack() << std::endl;

        std::cout << "Enter points at which to sample the spectrum, -999 to stop. " << std::endl;
        float w = 0.0f;
        while(true){
            std::cin >> w;
            if(w == -999.0f){
                break;
            }else{
                std::cout << "I(" << w << ") = " << I(w) << std::endl;
            }
        }

        float X,Y,Z,R,G,B;
        I.getXYZ(X,Y,Z);
        I.getRGB(R,G,B);

        std::cout << "Color vals: " << std::endl;
        std::cout << "XYZ = " << Point(X,Y,Z) << std::endl;
        std::cout << "RGB = " << Point(R,G,B) << std::endl;
    }
    */


}


#endif //TEST_SPECTRUM_H
