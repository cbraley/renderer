#ifndef UTILS_BITS_H
#define UTILS_BITS_H

#include <iostream>
#include <limits.h>
//--
#include "utils/Assert.h"

/**
 *  Utility code for various bit-maniuplation tricks.
 */
namespace Bits{

/// \brief Get system endianness.  A system is assumed to be either
///  big or little endian only.
///  In a big endian system the most significant(largest value) bit in a 
///  byte is first, whereas in a little endian system the most significant byte 
///  is last.
inline bool isBigEndian(){
    //dummy is 2 bytes, first all 0's second is all 0's then a final 1
    const short int dummy = 1;
    Assert(sizeof(short int) == 2); //Make sure shorts are 2 bytes on this platform

    //Extract the second byte of "dummy"
    const unsigned char b = ((unsigned char*)(&dummy))[0];

    //For a big endian system, b will be 00000001
    //whereas in a little endian system b will be 1000000 (in binary)
    const unsigned char BIG_ENDIAN_RESULT    = 1;
    const unsigned char LITTLE_ENDIAN_RESULT = 64;

    //If the below assertion failes, we are either on a middle
    //endian machine(astronomically unlikely...) or my code is broken
    Assert(b == BIG_ENDIAN_RESULT || b == LITTLE_ENDIAN_RESULT);

    return b == BIG_ENDIAN_RESULT;
}

inline void reverseBits(unsigned char* c){
    //TODO: Implemenet
}

void swapEndianness(unsigned char* buf, int numBytes){
    Assert(numBytes >= 0);
    const unsigned char* bufEnd = buf + numBytes;
    while(buf < bufEnd){
        reverseBits(buf++);
    }
    //TODO: Swap the endianness of this whole buffer
}

/// \brief Print out system low level properties to an ostream.
void printSystemInfo(std::ostream& os){
    os << "System Information: " << std::endl;
    os << "\tBig Endian: "       << isBigEndian()  << std::endl;
    os << "\tLittle Endian: "    << !isBigEndian() << std::endl;
    os << "\tBits in a char: "   << CHAR_BIT       << std::endl;

}

/// \brief Assert that this system is OK in terms of low level
///  expectations.
void validateSystem(){
    Assert(CHAR_BIT == 8);
}

}

#endif //UTILS_BITS_H
