#ifndef CUSTOM_TEST_REPORTER_H
#define CUSTOM_TEST_REPORTER_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
//--
#include <UnitTest++.h>
#include <TestReporter.h>
#include <TestDetails.h>
#include <Test.h>
#include <TestList.h>

using namespace UnitTest;

class CustomTestReporter : public TestReporter{
private:
    std::string getSepLineStr()const{
        return std::string("----------------------------------------------------------------------");
    }
    std::vector< std::pair<std::string, std::string> > failures;

public:
    virtual void ReportTestStart(TestDetails const& test){
        std::cout << "\tStarted: " << test.suiteName << ":" <<
            test.testName << std::endl;
    }

    virtual void ReportFailure(TestDetails const& test, char const* failure){
        std::cout << "\tTEST FAILURE -- " << failure << std::endl;
        std::cout << "\t\t" << test.suiteName << ":" << test.testName <<
            " line: " << test.lineNumber << " in file " << test.filename << std::endl;
        failures.push_back(std::pair<std::string,std::string>(test.suiteName, test.testName));
    }


    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed){
        std::cout << "\tFinished: " << test.testName << " in " << secondsElapsed << " s" << std::endl;
    }

    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed){
        std::cout << getSepLineStr() << std::endl;
        std::cout << "Test Summary: " << std::endl;
        if(failedTestCount > 0){
            //TODO: Print this list of failures in a cleaner, column aligned way
            std::cout << "FAILED " << failedTestCount << " of " << totalTestCount << " tests." << std::endl;
            for(size_t i = 0; i < failures.size(); i++){
                std::cout << "\t" << failures[i].first << "    " << failures[i].second << std::endl;
            }
            std::cout << "\tTotal Failures: " << failureCount << ", total seconds: " << secondsElapsed << std::endl;
        }else{
            std::cout << "PASSED all tests in: " << secondsElapsed << " seconds." << std::endl;
        }
    }

    int mCounter;
};


#endif //CUSTOM_TEST_REPORTER_H
