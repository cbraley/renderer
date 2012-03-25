#include <UnitTest++.h>

//TODO: Make some fancy makefile trick that takes all the files in ./test/* and includes
//them all here...
#include "testColorUtils.h"
#include "testSpectrum.h"
#include "testVector.h"
#include "testPoint.h"
#include "testTransform.h"
#include "testSphere.h"
#include "testImageSensor.h"
#include "testMatrixUtils.h"
#include "testStringUtils.h"
#include "testSphere.h"
#include "testSpectraCompare.h"
#include "testMatrixDenseKS.h"
#include "RNGs.h"
#include "testBoundingBox.h"
#include "testBRDFGeom.h"
//--
#include "CustomTestReporter.h"

using namespace UnitTest;

int main(){
    std::cout.precision( std::numeric_limits<double>::digits10 );
    std::cerr.precision( std::numeric_limits<double>::digits10 );

    RNG::initializeRNGs(); //Seed RNG

    //walk list of all tests, add those with a name that
    //matches one of the arguments  to a new TestList
    const TestList& allTests( Test::GetTestList() );

    CustomTestReporter reporter;
    TestRunner runner(reporter);
    return runner.RunTestsIf(allTests, 0, True(), 0 );
}

