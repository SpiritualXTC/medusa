#include "asset_directory.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = boost::filesystem;
using namespace medusa;



//
AssetDirectory::AssetDirectory(const std::string& path)
    : _path(path)
{

}


//
std::string AssetDirectory::readFile(const std::string& path)
{
    fs::path filePath = fs::path(_path + path);

    // Check the file exists and is a regular file
    if (!fs::exists(filePath))
    {
        throw MedusaError("File not found: " + filePath.string());
    }
    if (!fs::is_regular_file(filePath))
    {
        throw MedusaError("Not a regular file: " + filePath.string());
    }

    std::ifstream file(filePath.string());
    if (!file.is_open())
    {
        throw MedusaError("Failed to open: " + filePath.string());
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();;
}


//
std::vector<uint8_t> AssetDirectory::readBinary(const std::string& path)
{
    fs::path filePath = fs::path(_path + path);

    // Check the file exists and is a regular file
    if (!fs::exists(filePath))
    {
        throw MedusaError("File not found: " + filePath.string());
    }
    if (!fs::is_regular_file(filePath))
    {
        throw MedusaError("Not a regular file: " + filePath.string());
    }

    // Open the file at the end, simplifiy getting the size
    std::ifstream file(filePath.string(), std::ios::binary | std::ios::ate);

    // Get filesize
    auto size = file.tellg();
    file.seekg(0);

    // Read the blob
    std::vector<uint8_t> buf(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(buf.data()), size);

    return buf;
}
