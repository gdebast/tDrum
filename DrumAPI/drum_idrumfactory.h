#pragma once

#include "DrumAPI/drum_drumexception.h"
#include "Tools/tools_serializerhelper.h"
#include "Tools/tools_directory.h"
#include "Tools/tools_file.h"

#include <string>
#include <vector>
#include <fstream>
#include <memory>

namespace Drum
{
    template<class FactoryObject>
    class IDrumFactory
    {
    public:

        IDrumFactory(const std::string &fileExtension,
                     const Tools::Directory &workingDirectory) :
            m_fileExtension(fileExtension),
            m_workingDirectory(workingDirectory)
        {}

        virtual FactoryObject &getOneObject() = 0;

        /*
         * method which loads all objects from the extension.
         * the result can be returned using getObjects.
         */
        virtual void loadDirectory() final
        {
            auto files = m_workingDirectory.getFiles(m_fileExtension);
            for (const auto* file : files )
            {
                loadObject(file->getFullPath());
            }
        }

        /*
         * method which dumps all objects to their files.
         */
        void dumpToFile() const
        {

            for(const auto &[object,file] : m_AllCreatedObjects)
            {
                std::string serialized = object->getSerialized();

                std::ofstream flow(file);
                if(flow)
                {
                    flow << serialized;
                    flow.close();
                }
            }
        }

        /*
         * method returning all created objects
         */
        std::vector<FactoryObject*> getObjects() final
        {
            std::vector<FactoryObject*> objects;
            objects.reserve(m_AllCreatedObjects.size());
            for (const auto &[object,file] : m_AllCreatedObjects)
            {
                objects.push_back(object.get());
            }
            return objects;
        }

        std::string getDefaultFile() const
        {
            return m_workingDirectory.getDirectoryPath() + "/" + DEFAULTFILENAME + "." + m_fileExtension;
        }


    protected:
        std::vector<std::pair<std::unique_ptr<FactoryObject>,std::string>> m_AllCreatedObjects;

    private:
        std::string m_fileExtension;
        static const std::string DEFAULTFILENAME;
        const Tools::Directory &m_workingDirectory;

        FactoryObject& loadObject(const std::string& file)
        {

            // prepare the reading
            std::ifstream fileStream(file.c_str());

            DrumException::drumAssert(static_cast<bool>(fileStream),
                                      "Error from 'IDrumFactory::loadDrumTab': '{}' cannot be read.",
                                      file);

            // returned drumtab
            auto returnedDrumTab = std::make_unique<FactoryObject>();

            // read line per line
            std::string currentLine;
            std::string cumulativeLine;
            while (std::getline(fileStream,currentLine))
            {

                cumulativeLine += currentLine;
            }

            returnedDrumTab->fillFromSerialized(cumulativeLine);

            m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedDrumTab),file));
            return *(*m_AllCreatedObjects.rbegin()).first.get();
        }

    };
}

template<class FactoryObject>
const std::string Drum::IDrumFactory<FactoryObject>::DEFAULTFILENAME = "default";

