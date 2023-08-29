
#ifndef COLLIDERGAME_IO_H
#define COLLIDERGAME_IO_H
#include <string>
#include <vector>

// TODO: сделать асинохронными

class IO {
public:
    static bool readFile(const std::string& path, std::string& text);
    static bool readBinary(const std::string& path, std::vector<unsigned char>& data);
};


#endif //COLLIDERGAME_IO_H
