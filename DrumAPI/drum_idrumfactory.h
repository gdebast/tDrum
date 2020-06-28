#pragma once

#include "DrumAPI/drum_drumexception.h"
#include "Tools/tools_serializerhelper.h"
#include "Tools/tools_directory.h"
#include "Tools/tools_file.h"

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdio.h>

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

        virtual FactoryObject &createDefaultObject() = 0;


        /*
         * return one of the objects or create a default one.
         */
        virtual FactoryObject &getOneObject() final
        {
            // if one exists; return it
            if (m_AllCreatedObjects.size() > 0)
            {
                return *m_AllCreatedObjects[0].first.get();
            }

            // otherwise, create a default object
            return createDefaultObject();

        }


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
         * The files of the deleted objects are deleted too.
         */
        void dumpToFile() const
        {
            for(const auto &file : m_fileToRemove)
            {
                remove(file.c_str());
            }


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
         * methods which delete an Object
         */
        void deleteObject(FactoryObject &objectToDelete)
        {
            // copy all objects to a new vector
            std::vector<std::pair<std::unique_ptr<FactoryObject>,std::string>> newObjects;
            for (auto &[object,file] : m_AllCreatedObjects)
            {
                if(object.get() != &objectToDelete)
                {
                    newObjects.push_back(std::make_pair(std::move(object),file));
                }
                else
                {
                    m_fileToRemove.push_back(file);
                }
            }
            m_AllCreatedObjects = std::move(newObjects);
        }

        /*
         * method returning all created objects
         */
        virtual std::vector<FactoryObject*> getObjects() final
        {
            std::vector<FactoryObject*> objects;
            objects.reserve(m_AllCreatedObjects.size());
            for (const auto &[object,file] : m_AllCreatedObjects)
            {
                objects.push_back(object.get());
            }
            return objects;
        }

        unsigned getNumberOfInstances() const
        {
            return m_AllCreatedObjects.size();
        }

        /*
         * return a file name for an object.
         * The method garanties that the file does not exist.
         */
        virtual std::string getDefaultFile() const final
        {

            std::string fileFullPath;
            bool found(false);
            unsigned index(0);
            while(found == false)
            {
                fileFullPath = m_workingDirectory.getDirectoryPath()
                        + "/" + DEFAULTFILENAME + std::to_string(index)
                        + "." + m_fileExtension;
                found = getObjectFromFile(fileFullPath) == nullptr;
                index++;
            }

            return fileFullPath;

        }


    protected:
        std::vector<std::pair<std::unique_ptr<FactoryObject>,std::string>> m_AllCreatedObjects;
        const Tools::Directory &m_workingDirectory;

    private:
        std::string m_fileExtension;
        static const std::string DEFAULTFILENAME;
        std::vector<std::string> m_fileToRemove;

        FactoryObject* getObjectFromFile(const std::string &fileFullPath) const
        {
            for (const auto &[object,file] : m_AllCreatedObjects)
            {
                if (fileFullPath == file)
                {
                    return object.get();
                }
            }
            return nullptr;
        }

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

