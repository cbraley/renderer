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
    
    Assert(sizeof(int) > sizeof(char));
    //Should be true as long as we are not on a crazy embedded platform...
    //...but nevertheless the standard does not mandate this

    const int dummy = 1;
    if( *((char*)&dummy) == 1){
        //Little endian!
        return true;
    }else{
        //Big endian!
        return false;
    }
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
    Assert(CHAR_BIT == 8); //8 bits in a byte
    Assert(sizeof(int) > sizeof(char)); //ints are larger than chars
}

}

#endif //UTILS_BITS_H
