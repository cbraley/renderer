#ifndef TEST_IMAGE_SENSOR_H
#define TEST_IMAGE_SENSOR_H

#include <UnitTest++.h>
//--
#include "ImageSensor.h"
#include "Spectrum.h"
#include "ReconstructionFilter.h"
#include "GaussianFilter.h"
#include "BoxFilter.h"
#include "UtilsForTesting.h"
#include "ImageSampler.h"

SUITE(ImageSensorTests){

    TEST(ImOut){

        //In this test we generate a single random sampling pattern
        //then show images of the sensor with various reconstruction filters
        //and settings to verify they work.

        const int NUM_SAMPS = 300;
        const float SENSOR_W = 200.0f;
        const float SENSOR_H = 200.0f;
        const int NUM_SENSORS = 6;
        const std::string F_SUFFIX(".bmp");

        //Construct a few sensors with diff properties
        ReconstructionFilter* box_filter_11 = new BoxFilter(1.0f, 1.0f);
        ImageSensor sensor_box_11(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_E), box_filter_11);

        ReconstructionFilter* box_filter_15 = new BoxFilter(1.0f, 5.0f);
        ImageSensor sensor_box_15(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_F_11), box_filter_15);

        ReconstructionFilter* box_filter_82 = new BoxFilter(8.0f, 2.0f);
        ImageSensor sensor_box_82(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_E), box_filter_82);

        ReconstructionFilter* g_filter_1_55 = new GaussianFilter(1.0f, 5.0f, 5.0f);
        ImageSensor sensor_g_1_55(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_E), g_filter_1_55);

        ReconstructionFilter* g_filter_2_55 = new GaussianFilter(2.0f, 5.0f, 5.0f);
        ImageSensor sensor_g_2_55(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_F_7), g_filter_2_55);

        ReconstructionFilter* g_filter_p1_55 = new GaussianFilter(0.10f, 5.0f, 5.0f);
        ImageSensor sensor_g_p1_55(SENSOR_W, SENSOR_H, new Spectrum(Spectrum::CIE_ILLUM_E), g_filter_p1_55);

        //Pack it all into an array
        ImageSensor* sensorArr[NUM_SENSORS] =
            {&sensor_box_11, &sensor_box_15, &sensor_box_82,
            &sensor_g_1_55, &sensor_g_2_55, &sensor_g_p1_55
            };
        const std::string fNameArr[NUM_SENSORS]  = {"sensor_box_11", "sensor_box_15", "sensor_box_82",
            "sensor_gauss_a1_55", "sensor_gauss_a2_55", "sensor_gauss_apt1_55"
            };


        //Generate the sampling pattern and apply to all sensors
        Spectrum spec(Spectrum::CIE_ILLUM_F_1);
        for(int i = 0; i < NUM_SAMPS; i++){
            float x = randomFloat(0.0f, SENSOR_W);
            float y = randomFloat(0.0f, SENSOR_H);

            ImageSampler::Sample samp(x,y);
            //Add sample to EACH sensor
            for(int j = 0; j < NUM_SENSORS; j++){
                (sensorArr[j])->addSample(samp, &spec);
            }
        }

        //Write EACH sensor to disk
        for(int j = 0; j < NUM_SENSORS; j++){
            //CImg<float>* im = (sensorArr[j])->toImage(true);
            //im->save((TEST_TEMP_DIR + fNameArr[j] + F_SUFFIX).c_str());
            //delete im;
        }
    }

}

#endif //TEST_IMAGE_SENSOR_H
