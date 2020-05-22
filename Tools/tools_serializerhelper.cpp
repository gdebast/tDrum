#include "Tools/tools_serializerhelper.h"


namespace
{
    static const char END   = '}';
    static const char START = '{';
    static const char SETTINGSEPARATOR = '|';
    static const char SETTINGVALUESEPARATOR = '=';
}

using namespace Tools;

SerializerHelper::SerializerHelper()
{
}

SerializerHelper::SerializerHelper(const std::string &serializedString)
{
    initializeFromSerializedString(serializedString);
}

SerializerHelper::~SerializerHelper()
{
    for (auto& settingHelper : m_settingChildSerializerHelpers)
    {
        delete settingHelper.second;
        settingHelper.second = nullptr;
    }
}

bool SerializerHelper::isSerializerHelper(const std::string &setting) const
{
    for(const auto& settingHelper : m_settingChildSerializerHelpers)
    {
        if (settingHelper.first == setting)
        {
            return true;
        }
    }
    return false;
}

SerializerHelper &SerializerHelper::addSubSerializerHelper(const std::string &setting)
{
    SerializerHelper* newHelper = new SerializerHelper();
    std::pair<std::string,SerializerHelper*> pair(std::string(setting),newHelper);
    m_settingChildSerializerHelpers.push_back(std::move(pair));
    return *newHelper;
}

std::string SerializerHelper::getSerializedString() const
{
    std::string serializedString("");


    serializedString += START;

    // serialize the setting and value
    unsigned index(0);
    unsigned size(m_settingValue.size());
    bool hasSettingValue(m_settingValue.empty() == false);
    for (const auto& settingValue : m_settingValue)
    {

        serializedString += settingValue.first + SETTINGVALUESEPARATOR + settingValue.second;
        index++;
        if (index!=size)
        {
            serializedString += SETTINGSEPARATOR;
        }
    }

    // serialize the SerializerHelpers inside
    index = 0;
    size = m_settingChildSerializerHelpers.size();
    for (const auto& settingHelper : m_settingChildSerializerHelpers)
    {

        if (settingHelper.second)
        {
            if (index == 0 && hasSettingValue)
            {
                serializedString += SETTINGSEPARATOR;
            }

            serializedString += settingHelper.first + SETTINGVALUESEPARATOR + settingHelper.second->getSerializedString();
        }
        index ++;
        if (index!=size)
        {
            serializedString += SETTINGSEPARATOR;
        }


    }

    serializedString += END;

    return serializedString;

}

const SerializerHelper &SerializerHelper::getSubSerializerHelper(const std::string &setting) const
{
    for(const auto& settingHelper : m_settingChildSerializerHelpers)
    {
        if (settingHelper.first == setting)
        {
            return *settingHelper.second;
        }
    }

    // return this to indicate that we failed
    return *this;
}

void SerializerHelper::initializeFromSerializedString(const std::string &serializedString)
{

    std::string cumulativeSetting("");
    std::string cumulativeValue("");
    bool lookingForSetting_orValue(true); // true if we are looking for a setting, false if we are looking for a value
    bool settingFound(false);
    bool valueFound(false);

    auto it = serializedString.cbegin();

    while (it != serializedString.cend())
    {        
        const auto chr = *it;

        if (chr == SETTINGSEPARATOR)
        {
            lookingForSetting_orValue = true;
            settingFound = false;
            valueFound = false;
        }
        else if (chr == SETTINGVALUESEPARATOR)
        {
            lookingForSetting_orValue = false;
        }


        // prevents that any special character got added in the cumulative strings
        if (chr == END || chr == SETTINGVALUESEPARATOR || chr == SETTINGSEPARATOR)
        {
            it++;
            continue;
        }
        if (chr == START && it == serializedString.cbegin())
        {
            it++;
            continue;
        }

        // fill the cumulative string
        // --------------------------
        if (lookingForSetting_orValue == false )
        {
            cumulativeValue = "";
            unsigned endCharCounter(0);
            unsigned startCharCounter(0);
            while(it != serializedString.cend())
            {

                if (*it == END)
                {
                    endCharCounter++;
                }
                else if (*it == START)
                {
                    startCharCounter++;
                }

                cumulativeValue += *it;
                it++;

                // if at a moment, the number of start and end char encountered is equal
                // and we find a separator, it means we have to stop.
                if (startCharCounter == endCharCounter && (*it == END || *it == SETTINGSEPARATOR))
                {
                    break;
                }


            }
            valueFound = true;
        }
        else if(lookingForSetting_orValue == true)
        {
            cumulativeSetting = "";
            while(it != serializedString.cend() && *it != SETTINGVALUESEPARATOR)
            {
                cumulativeSetting += *it;
                it++;
            }

            settingFound = true;
        }

        // fill the found Setting-Value
        // ----------------------------
        if (settingFound && valueFound)
        {
            if (cumulativeValue.empty() == false && *(cumulativeValue.begin()) == START )
            {
                auto& subHelper = addSubSerializerHelper(cumulativeSetting);
                subHelper.initializeFromSerializedString(cumulativeValue);
            }
            else
            {
                m_settingValue.push_back(std::make_pair<std::string,std::string>(std::string(cumulativeSetting),std::string(cumulativeValue)));
            }
            lookingForSetting_orValue = true;
        }



    }

}





