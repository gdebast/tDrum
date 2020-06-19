#include "Tools/tools_directory.h"

#include "Tools/tools_file.h"

#include "boost/filesystem.hpp"


using namespace Tools;


Directory::Directory(const std::string &directoryPath) :
    m_directoryPath(directoryPath)
{
    // find all files in the directory
    boost::filesystem::path thisPath(m_directoryPath);
    for(const auto& entity : boost::filesystem::directory_iterator(thisPath))
    {
        boost::filesystem::path subPath = entity.path();
        if (boost::filesystem::is_regular_file(subPath))
        {
            m_filesInDirectory.push_back(std::make_unique<File>(subPath.filename().generic_string(),*this));
        }
    }

}

std::vector<const File *> Directory::getFiles(const std::string &extension) const
{
    std::vector<const File *> returnedFiles;
    for (const auto& fileUnique : m_filesInDirectory)
    {
        if (fileUnique->getFileExtension() == extension)
            returnedFiles.push_back(fileUnique.get());
    }
    return returnedFiles;
}

Directory::~Directory() = default;
