#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Tools
{
    class File;
}

namespace Tools
{
    class Directory
    {
        public:
            Directory(const std::string &directoryPath);
            Directory(const Directory&) = delete;
            Directory(Directory&&) = delete;
            Directory& operator=(const Directory&) = delete;
            Directory& operator=(Directory&&) = delete;

            ~Directory();

            std::string getDirectoryPath() const {return m_directoryPath;}
            std::vector<const File *> getFiles(const std::string& extension) const;


        private:
            std::string m_directoryPath;
            std::vector<std::unique_ptr<File>> m_filesInDirectory;
    };
}



