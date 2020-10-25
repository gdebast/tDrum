#pragma once

#include "Tools/tools_stringfunction_ext.h"

#include "exception"
#include "string"

namespace Tools {

    /*
     * class used to throw exception in the other APIs class
     */

    class Exception final : public std::exception{

    public:
        explicit Exception(const std::string& message) : m_excpetionMessage(message){}

        virtual const char* what() const noexcept override
        {
            return m_excpetionMessage.c_str();
        }

        template<class...Args>
        static void Assert(bool condition, const std::string& mainMessage, Args... others)
        {
            if (condition == false)
            {
                Tools::StringFunction strFunc;
                auto message =  strFunc.CFormatter(mainMessage,others...);
                throw Exception(message);
            }

        }

        static void Assert(bool condition, const std::string& message)
        {
            if (condition == false)
            {
                throw Exception(message);
            }

        }

    private:
        std::string m_excpetionMessage;

    };
}

