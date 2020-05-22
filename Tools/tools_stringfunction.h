#pragma once

#include <string>

namespace Tools
{
    class StringFunction
    {
    public:
        StringFunction();

        // to-string conversion method
        std::string toString(const bool& boolean);
        std::string toString(const std::string& str);
        std::string toString(const unsigned& value);
        std::string toString(const int& value);

        template<class T>
        std::string CFormatter(const std::string& mainString, const T& first);
        template<class T, class...Args>
        std::string CFormatter(const std::string& mainString, const T& first, Args... others);

        template<class T>
        T toValue(const std::string &toConvert);

        static const std::string BOOL_TRUE;
        static const std::string BOOL_FALSE;

    private:
        std::string replaceInString(const std::string& mainString,
                                    const std::string& toInsert,
                                    const std::string& toReplace);

    };
}



