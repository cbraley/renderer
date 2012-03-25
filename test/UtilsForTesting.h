#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string>
#include <cstring>

//Where to write temp files to if needed.
const std::string TEST_TEMP_DIR("test/tmp/");
const std::string TEST_DIR_SEP("/");

//Random float in a prescribed range (not that random)
inline float randomFloat(float a, float b){
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}

#endif //TEST_UTILS_H
