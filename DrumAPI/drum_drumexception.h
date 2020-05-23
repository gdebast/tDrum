#pragma once

#include "Tools/tools_stringfunction_ext.h"

#include "exception"
#include "string"

namespace Drum {

    /*
     * class used to throw exception in the other APIs class
     */

    class DrumException final : public std::exception{

    public:
        explicit DrumException(const std::string& message) : m_excpetionMessage(message){}

        virtual const char* what() const noexcept override
        {
            return m_excpetionMessage.c_str();
        }

        template<class...Args>
        static void drumAssert(bool condition, const std::string& mainMessage, Args... others)
        {
            if (condition)
            {
                Tools::StringFunction strFunc;
                auto message =  strFunc.CFormatter(mainMessage,others...);
                throw DrumException(message);
            }

        }

    private:
        std::string m_excpetionMessage;

    };
}

