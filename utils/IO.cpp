#include "IO.h"
#include <fstream>
#include "Logger.h"

bool IO::readFile(const std::string& path, std::string& text) {
    std::ifstream file(path);
    if (file.fail()) {
        Logger::error("[IO::readFile()] Error: Failed to open " + path);
        return false;
    }

    std::string line;
    while (std::getline(file, line))
        text += line + "\n";
    file.close();
    return true;
}

bool IO::readBinary(const std::string& path, std::vector<unsigned char>& data) {
    std::ifstream file(path, std::ios::binary);
    if (file.fail())
    {
        Logger::error(path);
        return false;
    }

    //Go to the end of file
    file.seekg(0, std::ios::end);

    //Get size of file
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    //Reduce size by header
    size -= file.tellg();

    data.resize(size);
    file.read((char *)&(data[0]), size);
    file.close();
    return true;
}