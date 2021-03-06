#ifndef GFX_ASSERT_H
#define GFX_ASSERT_H

#include <cassert>
#include <iostream>


/// \brief This file includes facilities for both "regular"
///  assertions that are turned off in release builds, as well
///  as "static" aka compile-time assertions which are inspired by
///  the Boost librarie's "static_assert()" (which is also in C++0x)
///


//TODO: FIGURE OUT

/// \brief Regular assertions
#ifndef NDEBUG //If debugging is on...
    #define Assert(x) if(!(x)){ \
                          std::cerr << "Assertion fail: " << std::endl << \
                          "\tExpression: " << #x << std::endl  <<         \
                          "\tFile: " <<  __FILE__ << std::endl <<         \
                          "\tLine: " <<  __LINE__ << std::endl;           \
                      }                                                   \
                      assert(x);
#else //If debugging is OFF...
    #define Assert(x) ((void)0)
#endif


/// \brief Compile time assertions
///  CTAssert = Compile Time Assert
template<bool>
struct CTAssert;

template<> struct CTAssert<true>{};

//How the above works:
//  At compile time we have expressions like:
//      CTAssert <1!=2 >();
//  which will use the specialization CTAssert<true>
//  However, things like:
//      CTAssert <1==2 >();
//  need a specialization for false, and thus these won't compile.
//  This works because CTAssert has no default implementation.

/// \brief Macro for making compile time assertions easier to read
#define COMPILE_TIME_ASSERT(expression) (CTAssert<(expression)>())

//TODO: Is there any way to make the error that gets generated by
//COMPILE_TIME_ASSERT more readable?  Currently its a template error that
//is confusing unless you are expecting potential failures of compile time
//assertions.  Maybe things like Boost do this???

#endif //GFX_ASSERT_H
