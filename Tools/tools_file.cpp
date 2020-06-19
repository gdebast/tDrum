#include "Tools/tools_file.h"
#include "Tools/tools_directory.h"

using namespace Tools;

File::File(const std::string &fileNameExtension,
           const Tools::Directory &parentDirectory) :
    m_parentDirectory(parentDirectory)
{
    std::string cumulativeStr{""};
    for (const auto& Char : fileNameExtension)
    {
        if (Char == '.')
        {
            m_fileName = cumulativeStr;
            cumulativeStr = "";
        }
        else
        {
            cumulativeStr += Char;
        }
    }
    m_fileExtension = cumulativeStr;

}

std::string File::getFullPath() const
{
    return m_parentDirectory.getDirectoryPath() + "/" + m_fileName + "." + m_fileExtension;
}
