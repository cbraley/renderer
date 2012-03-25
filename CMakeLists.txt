#------------------------------------------------------------------------------
#Main CMAKE file for Renderer
#Colin Braley
#
#    Version 1.0 - Oct 21, 2010
#    Version 2.0 - August 19, 2011 - Updated the cmake file to use many subdirectores inside src.
#
#TODO: Make debug and release versions
#TODO: Allow subdirectories in ./test
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
#Normally, you should only need to edit things in this section
#------------------------------------------------------------------------------

#The basics
cmake_minimum_required(VERSION 2.8.0)
project(CBRender)


#set(CMAKE_VERBOSE_MAKEFILE ON)

#Source files for utility code (these need to be MANUALLY edited)
set(UTILS_S2RGB_SRCS utils/src/s_to_rgb.cpp)

#Folders with library modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeModules)

#Locations of various important things on disk
set(SRC_DIR src)                #Root directory of all source code
set(TEST_DIR test)              #Root directory for unit test code
set(MAIN_SRC_FILE src/main.cpp) #File with main function for renderer

#Locations of output executables
set(CREN_APP_RELEASE               bin/cren_rel)        #Release binary
set(CREN_APP_RELEASE_WITH_DSYMBOLS bin/cren_rds)        #Release with debug symbols
set(CREN_APP_DEBUG                 bin/cren_debug)      #Debug binary
set(CREN_APP_TEST                  bin/cren_test)       #Test binary
#Utility programs
set(CREN_APP_SPEC2RGB              utils/bin/spec2rgb)  #Test binary

#TODO: Figure out how to do this
#set(ADDITIONAL_MAKE_CLEAN_FILES    gmon.out  test/tmp/* )

#Don't edit below this line under normal circumstances -----------------------

#------------------------------------------------------------------------------
#This section of code gets all source files (.cpp) in all subdirectories
#inside ./src.
#It prints info as it goes, and also adds each subdirectory to the
#INCLUDE_DIRECTORIES list.
#------------------------------------------------------------------------------

#Get a list of all subdirectories inside of the main source directory
set(SRC_FOLDERS_LIST "") #Keep a list of all directories containing source code
FILE(GLOB ALL_FILES ${SRC_DIR} ${SRC_DIR}/*)
FOREACH(_FILE ${ALL_FILES})
    IF(IS_DIRECTORY ${_FILE}) #We found a dir; add it to the list
        INCLUDE_DIRECTORIES(${_FILE})
        set(SRC_FOLDERS_LIST ${SRC_FOLDERS_LIST} ${_FILE})
    ENDIF()
ENDFOREACH()

#Print out all the subdirectories
message(STATUS "Source folders:")
FOREACH(CURR_FOLDER ${SRC_FOLDERS_LIST})
    message(STATUS "    ${CURR_FOLDER}")
ENDFOREACH()

#Collect all of the source files in each folder, and print them
#so we see which files are in which source directories
message(STATUS "Source files in each subdirectory:")
set(ALL_SOURCE_FILES "")
FOREACH(CURR_FOLDER ${SRC_FOLDERS_LIST})

    #Print the subdir we are in
    message(STATUS "    ${CURR_FOLDER}")

    #Get the soure files in the current subdir
    FILE(GLOB CURR_SRCS ${CURR_FOLDER}/*cpp)

    #Loop through all the files in that folder
    FOREACH(CURR_FILE ${CURR_SRCS})
        message(STATUS "        ${CURR_FILE}")
    ENDFOREACH()

    #Append to the source file list
    set(ALL_SOURCE_FILES ${ALL_SOURCE_FILES} ${CURR_SRCS})
ENDFOREACH()

#Print out all the source files, without directory info
message(STATUS "Final source file list:")
FOREACH(CURR_SRC_FILE ${ALL_SOURCE_FILES})
    message(STATUS "    ${CURR_SRC_FILE}")
ENDFOREACH()

#We now search for test source files; but we don't look into subdirectories since the
#./test folder has none
FILE(GLOB ALL_TEST_SOURCES ${TEST_DIR}/*cpp)
INCLUDE_DIRECTORIES(${TEST_DIR})

#Print out all the test sources
message(STATUS "Test folder: ${TEST_DIR}")
set(ALL_TEST_SOURCES ${ALL_TEST_SOURCES} ${ALL_SOURCE_FILES})


#Filter out the main renderer .cpp file from the tests
set(BAD_FILE "error")
set(TESTS_FILTERED "")
FOREACH(TEST_SRC_FILE ${ALL_TEST_SOURCES})
    IF(NOT "${TEST_SRC_FILE}" MATCHES ".*${MAIN_SRC_FILE}")
        set(TESTS_FILTERED ${TESTS_FILTERED} ${TEST_SRC_FILE})
    ELSE()
        message(STATUS "    Filtered out: ${TEST_SRC_FILE} from the test sources.")
        set(BAD_FILE ${TEST_SRC_FILE})
    ENDIF()
ENDFOREACH()
set(ALL_TEST_SOURCES ${TESTS_FILTERED})
message(STATUS "Source files for tests:")
FOREACH(TEST_SRC_FILE ${ALL_TEST_SOURCES})
    message(STATUS "    ${TEST_SRC_FILE}")
ENDFOREACH()


#Make sure not to include the .cpp file with the renderer's main function
#in the test app
list(REMOVE_ITEM ALL_TEST_SOURCES ${MAIN_SRC_FILE})

#Setup the source files for the utility spec2rgb
set(SPEC2RGB_SRCS ${ALL_SOURCE_FILES})
set(SPEC2RGB_SRCS ${SPEC2RGB_SRCS} ${UTILS_S2RGB_SRCS})
list(REMOVE_ITEM SPEC2RGB_SRCS ${BAD_FILE})


message(STATUS "Source files for spec2rgb:")
FOREACH(SF ${SPEC2RGB_SRCS})
    message(STATUS "    ${SF}")
ENDFOREACH()


#------------------------------------------------------------------------------
#Now that that is done; we must add our source files to the build
#------------------------------------------------------------------------------
set(GUI_CORE_SOURCES ${ALL_SOURCE_FILES})
list(REMOVE_ITEM GUI_CORE_SOURCES /home/cab6fh/Documents/Projects/Render_new/src/main.cpp) #TODO: Unhardcode!
list(REMOVE_ITEM GUI_CORE_SOURCES ~/Documents/Projects/Render_new/src/main.cpp) #TODO: Unhardcode!
list(REMOVE_ITEM GUI_CORE_SOURCES /home/cbraley/Documents/Projects/Render_1/src/main.cpp) #TODO: Unhardcode!
list(REMOVE_ITEM GUI_CORE_SOURCES /home/cbraley/Documents/Projects/git_ren/src/main.cpp) #TODO: Unhardcode!

add_subdirectory(gui)

set(CBRender_SRCS ${ALL_SOURCE_FILES})
set(CBRender_TEST_SRCS ${ALL_TEST_SOURCES})


#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
#add_executable(${CREN_APP_RELEASE}               ${CBRender_SRCS})
#add_executable(${CREN_APP_RELEASE_WITH_DSYMBOLS} ${CBRender_SRCS})
add_executable(${CREN_APP_DEBUG}                 ${CBRender_SRCS}      )
add_executable(${CREN_APP_TEST}                  ${CBRender_TEST_SRCS} )
add_executable(${CREN_APP_SPEC2RGB}              ${SPEC2RGB_SRCS}      )
#set(CMAKE_CXX_COMPILER clang)


#------------------------------------------------------------------------------
#Set compilation flags
#------------------------------------------------------------------------------
message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")
#TODO: What are visual studios flags like?
if (MSVC) #Visual Studio flags
    set ( CMAKE_CXX_FLAGS "/")
    set ( CMAKE_CXX_FLAGS_DEBUG "/")
    set ( CMAKE_CXX_FLAGS_RELEASE  "/" )
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

#find_package(PThreads REQUIRED)
#if(NOT PTHREAD_FOUND)
#    message(FATAL_ERROR "Need to have pthreads to build...")
#else()
#    include_directories(${PTHREAD_INCLUDE_DIR})
#    target_link_libraries(${CREN_APP_DEBUG} ${PTHREAD_LIBRARIES})
#endif(NOT PTHREAD_FOUND)



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

