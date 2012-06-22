#include <UnitTest++.h>
//--
#include <limits>
//--
#include "test/CustomTestReporter.h"
//--
//The file "test/tests_auto_generated.h" is automatically created
//during the build process and #include's each of the unit tests individually
//TODO: I need to figure out how to make this file be auto-generated
#include "test/unit_tests/tests_auto_generated.h"

using namespace UnitTest;

int main(){
    std::cout.precision( std::numeric_limits<double>::digits10 );
    std::cerr.precision( std::numeric_limits<double>::digits10 );

    //walk list of all tests, add those with a name that
    //matches one of the arguments  to a new TestList
    const TestList& allTests( Test::GetTestList() );

    CustomTestReporter reporter;
    TestRunner runner(reporter);
    return runner.RunTestsIf(allTests, 0, True(), 0 );
}

