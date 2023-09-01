#include "Logger.h"
#include <iostream>
using std::cout;


std::weak_ptr<Logger> Logger::getLogger() {
    auto instance = m_Instance;
    if (instance)
        return instance;

    std::shared_ptr<Logger> logger(new Logger());
    m_Instance = logger;
    return logger;
}

void Logger::info(const std::string& message) {
    getLogger();
    cout << "[INFO] " << message << "\n";
}

void Logger::error(const std::string& message) {
    getLogger();
    cout << "[ERROR] " << message << "\n";
}