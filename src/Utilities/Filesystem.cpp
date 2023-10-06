#include "Filesystem.h"

#include <filesystem>
#include <fstream>

namespace FileSystem
{

std::vector<char> loadTextFile(std::string path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    const auto size = std::filesystem::file_size(path);

    std::vector<char> data(size, '\0');
    file.read(data.data(), size);

    return data;
}

}