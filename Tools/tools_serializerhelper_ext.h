#pragma once

#include "Tools/tools_serializerhelper.h"
#include "Tools/tools_stringfunction_ext.h"

using namespace Tools;

template<class T>
void SerializerHelper::addToSerializeString(const std::string &setting, const T& value)
{
    // simply add a pair of setting-value in the container
    StringFunction strFunc;
    m_settingValue.push_back(std::make_pair<std::string,std::string>(std::string(setting),strFunc.toString(value)));
}

template<class T>
bool SerializerHelper::deserialize(const std::string &setting, T &value) const
{
    StringFunction strFunc;

    for (const auto& settingValue : m_settingValue)
    {
        if(settingValue.first == setting)
        {
            value = strFunc.toValue<T>(settingValue.second);
            return true;
        }
    }
    return false;
}
