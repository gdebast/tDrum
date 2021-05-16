#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Tools
{
    class Directory;
}

namespace Tools
{

    class DirectoryManager
    {
        public:
            DirectoryManager();
            DirectoryManager(const DirectoryManager&) = delete;
            DirectoryManager(DirectoryManager&&) = delete;
            DirectoryManager &operator=(const DirectoryManager&) = delete;
            DirectoryManager &operator=(DirectoryManager&&) = delete;

            ~DirectoryManager();

            //return the working directory of the application
            const Directory& getWorkingDirectory() const;

            //tells if a directory exists from a string
            bool isExistingDirectory(const std::string directoryStr) const;

        private:

            void initialize();

            std::unique_ptr<Directory> m_workingDirectory;

    };

}
