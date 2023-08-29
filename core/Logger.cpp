#include "Logger.h"
#include <iostream>
using std::cout;


void Logger::info(const std::string& message) {
    cout << "[INFO]" << message << "\n";
}

void Logger::error(const std::string& message) {
    cout << "[ERROR]" << message << "\n";
}