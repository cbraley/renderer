#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>

/**
 *  Miscellaneous functions for common operations on
 *  STL std::strings.
 *  @author Colin Braley
 *  @date October 13, 2011
 */
namespace StringUtils{


// \brief Return a std::string that is a list of characters in some char-set.
enum CommonCharSet{ WHITESPACE_CSET, NUMBERS_CSET};
std::string getCharSet(CommonCharSet set);


/// \brief Check if character c matches any found in matchList.
bool matchesAny(const char c, const std::string& matchList);


/**
 *  Split the string "input" into tokens(substrings).
 *
 *  Tokens characters for which the predicate tokPredFPtr returns true.
 *
 *  tokPredFPtr is a function that takes a single char as an argument and returns a bool.
 */
std::vector<std::string> tokenize(const std::string& input, bool (*tokPredFPtr)(char));


/**
 *  Split the string "input" into tokens(substrings).
 *
 *  Tokens are strings that are separated by ANY CHARACTER in tokenCharList.
 *  Tokens of length 0 are not returned.
 *
 *  Written because strtok is C-ish and not thread safe.  strtok_r is thread safe,
 *  but still unweildy and not always available.
 */
std::vector<std::string> tokenize(const std::string& input, const std::string& tokenCharList);


/**
 *  Convert any object that has a properly defined << operator
 *  (stream insertion, not bitshift) into a std::string.
 *  This is defined inline because its a template function.
 */
template<typename T>
std::string stringify(const T& t){
    std::stringstream stream;
    stream << t;
    return stream.str();
}


/**
 *  Check if a string starts with a given prefix.
 */
bool startsWith(const std::string& testMe, const std::string& prefix);

/**
 *  Check if a string ends with a given suffix.
 */
bool endsWith(const std::string& testMe, const std::string& suffix);

/**
 * Trim whitespace off of the front of a string.
 *  @param in is the string to trim.
 *  @return a trimmed string
 */
std::string trimFront(const std::string& in);


/**
 * Trim whitespace off of the back of a string.
 *  @param in is the string to trim.
 *  @return a trimmed string
 */
std::string trimBack(const std::string& in);

/**
 * Trim whitespace off of both ends of a string
 *  @param in is the string to trim.
 *  @return a trimmed string
 */
std::string trim(const std::string& in);

/// \brief Reverse a string in place.
void reverse(std::string& str);

/// \brief Get a new reversed version of a string.
std::string getReversed(const std::string& str);

}

#endif //STRING_UTILS_H
