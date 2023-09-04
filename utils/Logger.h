#ifndef COLLIDERGAME_LOGGER_H
#define COLLIDERGAME_LOGGER_H

#include <string>
#include <fstream>
#include <memory>

/** Logger is a singleton class */
class Logger {
public:

    static std::weak_ptr<Logger> getLogger();

    static std::weak_ptr<Logger> getFileLogger(const std::string& logFilePath);

    static void info(const std::string& message);

    static void warning(const std::string& message);

    static void error(const std::string& message);

protected:
    Logger();
    Logger(const std::string& logFilePath);
    virtual ~Logger();

    void fileInfo(const std::string& message);
    void fileWarning(const std::string& message);
    void fileError(const std::string& message);

protected:
    static std::shared_ptr<Logger> m_Instance;
    std::shared_ptr<std::ofstream> m_OFstream;
};

#endif //COLLIDERGAME_LOGGER_H
