#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Tools
{
    class SerializerHelper
    {
        public:
            explicit SerializerHelper();
            explicit SerializerHelper(const std::string& serializedString);
            ~SerializerHelper();
            SerializerHelper(const SerializerHelper&) = delete;
            SerializerHelper(SerializerHelper&&) = delete;
            SerializerHelper& operator=(const SerializerHelper&) = delete;
            SerializerHelper& operator=(SerializerHelper&&) = delete;

            /*
             * serialization and deserialization of string values
             */
            template<class T>
            void addToSerializeString(const std::string &setting, const T& value);
            template<class T>
            bool deserialize(const std::string &setting, T& value) const;

            /*
             * tells if the setting value is a SerializerHelper
             */
            bool isSerializerHelper(const std::string &setting) const;

            /*
             * function used to add a new empty SerializerHelper in this SerializerHelper
             */
            SerializerHelper& addSubSerializerHelper(const std::string &setting);

            /*
             * get the conversion of the Serializer helper into string
             */
            std::string getSerializedString() const;

            /*
             * get a sub serializer helper which is contained in a setting
             */
            const SerializerHelper& getSubSerializerHelper(const std::string &setting) const;



        private:

            void initializeFromSerializedString(const std::string& serializedString);

            std::vector<std::pair<std::string,std::string>>        m_settingValue{};
            std::vector<std::pair<std::string,SerializerHelper*>>  m_settingChildSerializerHelpers{};

    };
}


