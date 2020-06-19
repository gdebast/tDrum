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
            Directory& getWorkingDirectory();

        private:

            void initialize();

            std::unique_ptr<Directory> m_workingDirectory;

    };

}
