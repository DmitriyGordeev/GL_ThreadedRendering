#ifndef COLLIDERGAME_LOGGER_H
#define COLLIDERGAME_LOGGER_H

#include <string>
#include <memory>

class Logger {
public:
//    static void info(const std::string& message);
//    static void error(const std::string& message);

    static std::weak_ptr<Logger> getLogger();
    static std::shared_ptr<Logger> m_Instance;
protected:
    Logger() = default;

};

#endif //COLLIDERGAME_LOGGER_H
