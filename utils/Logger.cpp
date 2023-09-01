#include "Logger.h"
#include <iostream>
using std::cout;


//void Logger::info(const std::string& message) {
//    cout << "[INFO] " << message << "\n";
//}
//
//void Logger::error(const std::string& message) {
//    cout << "[ERROR] " << message << "\n";
//}


std::weak_ptr<Logger> Logger::getLogger() {
    auto instance = m_Instance;
    if (instance)
        return instance;

    std::shared_ptr<Logger> logger(new Logger());
    m_Instance = logger;
    return logger;
}