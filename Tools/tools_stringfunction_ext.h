#pragma once

#include "Tools/tools_stringfunction.h"

namespace
{

    // toValue methods
    template<class T>
    T implToValue(const std::string& str) = delete;
    template<>
    std::string implToValue<std::string>(const std::string& str)
    {
        return str;
    }
    template<>
    bool implToValue<bool>(const std::string& str)
    {
        if(str == Tools::StringFunction::BOOL_TRUE)
            return true;
        else
            return false;
    }
    template<>
    unsigned implToValue<unsigned>(const std::string& str)
    {
        return std::stoi(str);
    }
    template<>
    int implToValue<int>(const std::string& str)
    {
        return std::stoi(str);
    }

}

using namespace Tools;
template<class T>
std::string StringFunction::CFormatter(const std::string& mainString, const T& first)
{
    return  replaceInString(mainString,toString(first),"{}");
}
template<class T, class...Args>
std::string StringFunction::CFormatter(const std::string& mainString, const T& first, Args... others)
{
    auto newMainString = replaceInString(mainString,toString(first),"{}");
    return  CFormatter(newMainString,others...);
}


template<class T>
T StringFunction::toValue(const std::string &toConvert)
{
    return implToValue<T>(toConvert);
}
