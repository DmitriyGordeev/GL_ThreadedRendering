#include "Logger.h"
#include <iostream>
using std::cout;

Logger::Logger() = default;

Logger::Logger(const std::string& logFilePath) {
    m_OFstream = std::make_shared<std::ofstream>(logFilePath);
}

Logger::~Logger() = default;

std::weak_ptr<Logger> Logger::getLogger() {
    auto instance = m_Instance;
    if (instance)
        return instance;

    std::shared_ptr<Logger> logger(new Logger(), [](Logger* obj) {
        if (obj->m_OFstream)
            obj->m_OFstream->close();
    });
    m_Instance = logger;
    return logger;
}

std::weak_ptr<Logger> Logger::getFileLogger(const std::string& logFilePath) {
    auto instance = m_Instance;
    if (instance)
        return instance;

    std::shared_ptr<Logger> logger(new Logger(logFilePath),
                                   [](Logger* obj){
        // Custom deleter for the shared_ptr -
        // need to close ofstream manually on Logger's deletion
        if (obj->m_OFstream)
            obj->m_OFstream->close();
    });
    m_Instance = logger;
    return logger;
}

void Logger::info(const std::string& message) {
    auto logger = getLogger();
    cout << "[INFO] " << message << "\n";
    logger.lock()->fileInfo(message);
}

void Logger::warning(const std::string& message) {
    auto logger = getLogger();
    cout << "[WARNING] " << message << "\n";
    logger.lock()->fileWarning(message);
}

void Logger::error(const std::string& message) {
    auto logger = getLogger();
    cout << "[ERROR] " << message << "\n";
    logger.lock()->fileInfo(message);
}

void Logger::fileInfo(const std::string& message) {
    if (m_OFstream)
        (*m_OFstream) << "[INFO] " << message << "\n";
}

void Logger::fileWarning(const std::string& message) {
    if (m_OFstream)
        (*m_OFstream) << "[WARNING] " << message << "\n";
}

void Logger::fileError(const std::string& message) {
    if (m_OFstream)
        (*m_OFstream) << "[ERROR] " << message << "\n";
}