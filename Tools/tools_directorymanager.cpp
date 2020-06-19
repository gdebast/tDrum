#include "Tools/tools_directorymanager.h"

#include "Tools/tools_directory.h"

#include "boost/filesystem.hpp"
#include <string>


using namespace Tools;

DirectoryManager::DirectoryManager()
{
    initialize();
}

Directory &DirectoryManager::getWorkingDirectory()
{
    return *m_workingDirectory;
}

void DirectoryManager::initialize()
{

    // find the root of the working directory
    std::string workingDirectoryRootName = std::getenv("USERPROFILE");
    boost::filesystem::path workingDirectoryRootPath(workingDirectoryRootName);
    if (boost::filesystem::exists(workingDirectoryRootPath) == false)
    {
        workingDirectoryRootPath = boost::filesystem::current_path();
    }

    // find the working directory
    const std::string workingDirectoryName = "/tDrum";
    boost::filesystem::path workingDirectoryPath(workingDirectoryRootPath.generic_string() + workingDirectoryName);
    if (boost::filesystem::exists(workingDirectoryPath) == false)
    {
        boost::filesystem::create_directory(workingDirectoryPath);
    }

    m_workingDirectory = std::make_unique<Directory>(workingDirectoryPath.generic_string());

}

DirectoryManager::~DirectoryManager() = default;
