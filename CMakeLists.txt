#------------------------------------------------------------------------------
#Main CMAKE file for Renderer
#Colin Braley
#
#    Version 1.0 - Oct 21, 2010
#    Version 2.0 - August 19, 2011 - Updated the cmake file to use many subdirectores inside src.
#    Version 3.0 - June 20 2012 - Completely redone to remove cruft and use built in cmake features instead of reinventing the wheel.
#    All #include's now must explicitly specify the sub-directory in which the resulting .h file is in.  This is a better strategy for large projects 
#    to avoid filename name-clashes and to enhance readability.
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#Normally, you should only need to edit things in this section
#------------------------------------------------------------------------------

#The basics
cmake_minimum_required(VERSION 2.8.0)
project(CBRender)
#set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

#Folders with library modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeModules)

#Locations of output executables
set(CREN_APP_RELEASE               bin/cren_rel)      #Release binary
set(CREN_APP_RELEASE_WITH_DSYMBOLS bin/cren_rds)      #Release with debug symbols
set(CREN_APP_DEBUG                 bin/cren_debug)    #Debug binary
set(CREN_APP_TEST                  bin/cren_test)     #Test binary
set(CREN_APP_GUI                   bin/cren_gui)      #GUI binary


#TODO: Figure out how to do this
#set(ADDITIONAL_MAKE_CLEAN_FILES    gmon.out  test/tmp/* )

#Root directory in which all code is located
set(BASE_SOURCE_DIR src)
set(GUI_BASE_PATH "${BASE_SOURCE_DIR}/gui")

#Location of unit tests
set(UNIT_TEST_LOC "${BASE_SOURCE_DIR}/test/unit_tests")

#Path to auto-generated file of unit tests(see main_test.cpp)
set(AUTOGENED_UNITTEST_FILE_NAME "${UNIT_TEST_LOC}/tests_auto_generated.h")
set(AUTOGENED_UNITTEST_COMMAND "./${BASE_SOURCE_DIR}/scripts/autogenerate_unit_test_header.py")

message(STATUS "Command is:  ${AUTOGENED_UNITTEST_COMMAND}")
message(STATUS "Out file is: ${AUTOGENED_UNITTEST_FILE_NAME}")

#Get all unit test files
file(GLOB_RECURSE ALL_UNITTESTS_RECURSIVE ${UNIT_TEST_LOC}/test*.h)

#Fix all paths to be relative
set(ALL_TEST_FILES_REL "")
foreach(CURR_FILE ${ALL_UNITTESTS_RECURSIVE})
    file(RELATIVE_PATH temp ${CMAKE_SOURCE_DIR}/${BASE_SOURCE_DIR} ${CURR_FILE})
    list(APPEND ALL_TEST_FILES_REL ${temp})
endforeach()

#Print all test files
message(STATUS "List of unit tests(located in ${UNIT_TEST_LOC}):")
foreach(FILE ${ALL_TEST_FILES_REL})
    message(STATUS "\t${FILE}")
endforeach()


#Don't edit below this line under normal circumstances -----------------------

include_directories(${BASE_SOURCE_DIR})
message(STATUS "Path to root source directory: ${BASE_SOURCE_DIR}")

#Each .cpp file that contains an int main(int, char**) function is listed below
set(MAIN_CPP_CMD_LINE "${BASE_SOURCE_DIR}/main_cmd_line.cpp") 
set(MAIN_CPP_GUI "${BASE_SOURCE_DIR}/main_gui.cpp")
set(MAIN_CPP_UNIT_TESTS "${BASE_SOURCE_DIR}/main_test.cpp")
#List of all files containing main(int, char**) functions
set(MAIN_CPP_LIST_ALL ${MAIN_CPP_CMD_LINE} ${MAIN_CPP_GUI} ${MAIN_CPP_UNIT_TESTS})
#Print list of all "main" files
message(STATUS "All the main files are: ${MAIN_CPP_LIST_ALL}")


#Recursively find all .cpp files
file(GLOB_RECURSE ALL_CPP_FILES_RECURSIVE ${BASE_SOURCE_DIR}/*.cpp)
#Now convert all file paths into paths relative to the project root directory
set(ALL_CPP_FILES_REL "")
foreach(CURR_FILE ${ALL_CPP_FILES_RECURSIVE})
    file(RELATIVE_PATH temp ${CMAKE_SOURCE_DIR} ${CURR_FILE})
    list(APPEND ALL_CPP_FILES_REL ${temp})
endforeach()


#Remove the files containing a main function from the list of .cpp files
#If we left these in, we get multiple entry points(main functions) error when compiling
foreach(CURR_MAIN_FILE ${MAIN_CPP_LIST_ALL})
    list(REMOVE_ITEM ALL_CPP_FILES_REL ${CURR_MAIN_FILE})
endforeach()

#We want two lists
#    ALL_CPP_FILES_REL,     which is all .cpp files that are NOT in the GUI
#    ALL_CPP_GUI_FILES_REL, which is all .cpp files that ARE used in the GUI
set(ALL_CPP_GUI_FILES_REL     "")
set(ALL_CPP_NON_GUI_FILES_REL "")
foreach(FILE ${ALL_CPP_FILES_REL})
    if("${FILE}" MATCHES "${GUI_BASE_PATH}.*")
        list(APPEND ALL_CPP_GUI_FILES_REL     ${FILE})
    else()
        list(APPEND ALL_CPP_NON_GUI_FILES_REL ${FILE})
    endif()
endforeach()
set(ALL_CPP_FILES_REL ${ALL_CPP_NON_GUI_FILES_REL})



#Print list of all source files
message(STATUS "All source files(excluding files with a main function and GUI code):")
foreach(FILE ${ALL_CPP_FILES_REL})
    message(STATUS "\t${FILE}")
endforeach()


#Print list of all source files for the GUI
message(STATUS "All source files(including GUI files, exluding files with a main function):")
foreach(FILE ${ALL_CPP_GUI_FILES_REL})
    message(STATUS "\t${FILE}")
endforeach()














#QT Related things for the GUI app
set(QT_USE_QTOPENGL TRUE)
find_package(Qt4 REQUIRED)

#Find files we need to invoke QT MOC on
set(MOC_FILES_LOC "${BASE_SOURCE_DIR}/gui")
file(GLOB_RECURSE QT_FILES_TO_MOC ${MOC_FILES_LOC}/*.h)

#Invoke MOC
message(STATUS "Will apply QT MOC to:")
foreach(FILE ${QT_FILES_TO_MOC})
    message(STATUS "\t${FILE}")
endforeach()
QT4_WRAP_CPP(QT_MOCED_HEADERS ${QT_FILES_TO_MOC})

#QT includes
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})




#Write to the auto-generated include file
add_custom_command(
    TARGET ${CREN_APP_TEST} PRE_BUILD
    COMMAND "${AUTOGENED_UNITTEST_COMMAND}"
    ARGS ""
    COMMENT "Autogenerating unit test file: ${AUTOGENED_UNITTEST_FILE_NAME}" 
    )


#Add executables
#    Command line renderer
add_executable(${CREN_APP_DEBUG} ${ALL_CPP_FILES_REL} ${MAIN_CPP_CMD_LINE}  )
message(STATUS "Added executable: ${CREN_APP_DEBUG}")
#    Unit test program
add_executable(${CREN_APP_TEST}  ${ALL_CPP_FILES_REL} ${MAIN_CPP_UNIT_TESTS})
message(STATUS "Added executable: ${CREN_APP_TEST}")
#    GUI executable
add_executable(${CREN_APP_GUI} ${ALL_CPP_FILES_REL} ${QT_MOCED_HEADERS} ${ALL_CPP_GUI_FILES_REL} ${MAIN_CPP_GUI}) #Note that the MOCed headers go here

#Link QT
target_link_libraries(${CREN_APP_GUI} ${QT_LIBRARIES})

#------------------------------------------------------------------------------
#Set compilation flags
#------------------------------------------------------------------------------
message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")
#TODO: What are visual studios flags like?
if (MSVC) #Visual Studio flags
    set ( CMAKE_CXX_FLAGS "")
    set ( CMAKE_CXX_FLAGS_DEBUG "")
    set ( CMAKE_CXX_FLAGS_RELEASE  "" )
endif ()

if (CMAKE_COMPILER_IS_GNUCXX) #g++ flags
    set ( CMAKE_CXX_FLAGS "-Wall -m64 -Wextra -ansi -pedantic -msse3 -Wextra -Wno-long-long -lpthread") #TODO: Get pthreads working!
    set ( CMAKE_CXX_FLAGS_DEBUG "-g -fno-inline -O0 -Wall -pg -ggdb")
    set ( CMAKE_CXX_FLAGS_RELEASE  "-O3 -funroll-loops -ffast-math -finline-functions" )
endif ()

if (CMAKE_COMPILER_IS_CLANG) #clang flags
    set ( CMAKE_CXX_FLAGS "-fpermissive")
endif()


#------------------------------------------------------------------------------
#Deal with any external libraries
#------------------------------------------------------------------------------


#pthreads---------------------------------------------------------------------
find_package(Threads REQUIRED)
target_link_libraries(${CREN_APP_DEBUG} ${CMAKE_THREAD_LIBS_INIT})


#UnitTest++ ------------------------------------------------------------------
set(UTPP_DIR lib/unittest++) #include and bin
include_directories(${UTPP_DIR}/include)
find_library(UTPP_LIBRARY
    NAMES UnitTest++
    PATHS ${UTPP_DIR}/bin /usr/lib /usr/local/lib
)
if(UTPP_LIBRARY)
    target_link_libraries(${CREN_APP_TEST} ${UTPP_LIBRARY})
else()
    message(FATAL_ERROR "Could not find unittest++")
endif()

