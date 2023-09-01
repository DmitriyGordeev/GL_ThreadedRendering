#ifndef COLLIDERGAME_LOGGER_H
#define COLLIDERGAME_LOGGER_H

#include <string>
#include <memory>

/** Logger is a singleton class */
class Logger {
public:

    static std::weak_ptr<Logger> getLogger();

    static void info(const std::string& message);

    static void error(const std::string& message);

protected:
    static std::shared_ptr<Logger> m_Instance;
    Logger() = default;

};

#endif //COLLIDERGAME_LOGGER_H
