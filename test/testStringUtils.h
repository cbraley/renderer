#ifndef TEST_STRING_UTILS_H
#define TEST_STRING_UTILS_H

#include <UnitTest++.h>
//--
#include <vector>
#include <string>
#include <algorithm>
//--
#include "StringUtils.h"

using namespace StringUtils;

SUITE(StringUtilsTests){

    bool alwaysTrue(char c){ return c==c; }
    bool alwaysFalse(char c){ return c!=c; }

    TEST(StringTokenize){

        //--------
        std::string tokenizeMe("    \tcat dog\tfish   elephant\t\t\t\r\n");
        std::vector<std::string> toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        //--
        CHECK(toks.size() == 4);
        CHECK( std::find(toks.begin(), toks.end(), std::string("cat")) != toks.end());
        CHECK( std::find(toks.begin(), toks.end(), std::string("dog")) != toks.end());
        CHECK( std::find(toks.begin(), toks.end(), std::string("fish")) != toks.end());
        CHECK( std::find(toks.begin(), toks.end(), std::string("elephant")) != toks.end());
        //--------
        tokenizeMe = std::string("  \t  \t\t\t\r\n");
        toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        CHECK(toks.size() == 0);
        //--------
        tokenizeMe = std::string("d");
        toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        CHECK(toks.size() == 1);
        CHECK(toks[0] == std::string("d"));
        //--------
        tokenizeMe = std::string("cat  ");
        toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        CHECK(toks.size() == 1);
        CHECK(toks[0] == std::string("cat"));
        //--------
        tokenizeMe = std::string("  cat");
        toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        CHECK(toks.size() == 1);
        CHECK(toks[0] == std::string("cat"));
        //--------
        tokenizeMe = std::string("cat   ");
        toks = tokenize(tokenizeMe, getCharSet(StringUtils::WHITESPACE_CSET));
        CHECK(toks.size() == 1);
        CHECK(toks[0] == std::string("cat"));
        //--------
        tokenizeMe = std::string("cat  grhr rhrh \rgrhrhrfhb ");
        toks = tokenize(tokenizeMe, alwaysTrue);
        CHECK(toks.size() == 0);
        //--------
        tokenizeMe = std::string("cat  grhr rhrh \rgrhrhrfhb ");
        toks = tokenize(tokenizeMe, alwaysFalse);
        CHECK(toks.size() == 1);
        CHECK(toks[0] == std::string("cat  grhr rhrh \rgrhrhrfhb "));
    }

    TEST(StringStartsWith){
        CHECK(startsWith("cat", "cat"));
        CHECK(startsWith("cat", ""));
        CHECK(startsWith("", ""));
        CHECK(startsWith("dog", "d"));
        CHECK(!startsWith("log","logger"));
    }

    TEST(StringTrim){
        CHECK(trim("   log     \t") == std::string("log"));
        CHECK(trim("\tlog\t")       == std::string("log"));
        CHECK(trim("log")           == std::string("log"));
        CHECK(trim("\t\t\n   ")     == std::string(""));
        CHECK(trim("") == std::string(""));
    }
   
    TEST(StringReverse){
        CHECK(getReversed("") == std::string(""));
        CHECK(getReversed("abc") == std::string("cba"));
        CHECK(getReversed("c") == std::string("c"));
        CHECK(getReversed("amanaplanacanalpanama") == std::string("amanaplanacanalpanama"));
    }



}

#endif //TEST_STRING_UTILS_H
