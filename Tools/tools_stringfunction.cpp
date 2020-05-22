#include "Tools/tools_stringfunction.h"


using namespace Tools;

const std::string StringFunction::BOOL_TRUE  = "1";
const std::string StringFunction::BOOL_FALSE = "0";

StringFunction::StringFunction()
{}

std::string StringFunction::toString(const std::string& str)
{
    return str;
}

std::string StringFunction::toString(const bool& boolean)
{
    if(boolean)
        return BOOL_TRUE;
    else
        return BOOL_FALSE;
}

std::string StringFunction::toString(const unsigned& value)
{
    return std::to_string(value);
}
std::string StringFunction::toString(const int& value)
{
    return std::to_string(value);
}

std::string StringFunction::replaceInString(const std::string &mainString,
                                            const std::string &toInsert,
                                            const std::string &toReplace)
{
    auto pos = mainString.find(toReplace);
    if (pos == std::string::npos)
        return mainString;

    auto newStr = mainString;
    newStr.replace(pos, toReplace.size(),toInsert);

    return newStr;

}
