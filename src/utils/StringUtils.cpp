#include "utils/StringUtils.h"

#include "utils/Assert.h"

enum CommonCharSet{ WHITESPACE, NUMBERS };

static const std::string WHITESPACE_LIST(" \t\n\v\f\r");
static const std::string NUMBER_LIST    ("0123456789");


std::string StringUtils::getCharSet(CommonCharSet set){
    switch(set){
        case WHITESPACE_CSET:
            return WHITESPACE_LIST;
        case NUMBERS_CSET:
            return NUMBER_LIST;
        default:
            Assert(false);
            return "";
    }
}

bool StringUtils::matchesAny(const char c, const std::string& matchList){
    for(size_t i = 0; i < matchList.size(); i++){
        if(c == matchList[i]){
            return true;
        }
    }
    return false;
}


std::vector<std::string> StringUtils::tokenize(const std::string& input, bool (*tokPredFPtr)(char))
{
    std::vector<std::string> ret;
    std::string soFar("");

    for(size_t i = 0; i < input.size(); i++){
        const char curr = input[i];
        if((*tokPredFPtr)(curr)){ //We found the end of a token
            if(!soFar.empty()){ //Skip empty tokens
                ret.push_back(soFar);
                soFar = std::string("");
            }
        }else{ //Add to the current list
            soFar += curr;
        }
    }
    if(!soFar.empty()){
        ret.push_back(soFar);
    }

    return ret;
}

std::vector<std::string>
StringUtils::tokenize(const std::string& input,
    const std::string& tokenCharList)
{
    std::vector<std::string> ret;
    std::string soFar("");

    for(size_t i = 0; i < input.size(); i++){
        const char curr = input[i];
        if(matchesAny(curr, tokenCharList)){ //We found the end of a token
            if(!soFar.empty()){ //Skip empty tokens
                ret.push_back(soFar);
                soFar = std::string("");
            }
        }else{ //Add to the current list
            soFar += curr;
        }
    }
    if(!soFar.empty()){
        ret.push_back(soFar);
    }

    return ret;
}

bool StringUtils::startsWith(const std::string& testMe, const std::string& prefix){
    return testMe.find(prefix) == 0;
}


bool StringUtils::endsWith(const std::string& testMe, const std::string& suffix){
    return testMe.rfind(suffix) == (testMe.size() - (suffix.size()));
}

std::string StringUtils::trimFront(const std::string& in){
    size_t i = 0;
    for(i = 0; i < in.size(); i++){
        const char curr = in[i];
        if(!StringUtils::matchesAny(curr, WHITESPACE_LIST)){
            break;
        }
    }
    return (in.empty()) ? in : (in.substr(i, std::string::npos));
}

std::string StringUtils::trimBack(const std::string& in){
    return StringUtils::getReversed(StringUtils::trimFront(StringUtils::getReversed(in)));
}


std::string StringUtils::trim(const std::string& in){
    return StringUtils::trimBack(StringUtils::trimFront(in));
}


void StringUtils::reverse(std::string& str){
    for(size_t i = 0; i < str.size() / 2; i++){
        const size_t index = str.size() - i - 1;
        char temp = str[i];
        str[i] = str[index];
        str[index] = temp;
    }
}

std::string StringUtils::getReversed(const std::string& str){
    std::string temp(str);
    StringUtils::reverse(temp);
    return temp;
}



