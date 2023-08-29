#ifndef COLLIDERGAME_LOGGER_H
#define COLLIDERGAME_LOGGER_H

#include <string>

class Logger {
public:
    static void info(const std::string& message);
    static void error(const std::string& message);
};

#endif //COLLIDERGAME_LOGGER_H
