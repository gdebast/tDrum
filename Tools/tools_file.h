#pragma once

#include <string>

namespace Tools
{
    class Directory;
}

namespace Tools
{
    class File
    {
        public:
            File(const std::string &fileNameExtension,const Directory& parentDirectory);
            File(const File&) = delete;
            File(File&&) = delete;
            File& operator=(const File&) = delete;
            File& operator=(File&&) = delete;

            ~File() = default;

            const Directory &getDirectory() const {return m_parentDirectory;}
            std::string getFileName() const {return m_fileName;}
            std::string getFileExtension() const {return m_fileExtension;}


        private:
            std::string m_fileName{""};
            std::string m_fileExtension{""};
            const Directory &m_parentDirectory;
    };
}


