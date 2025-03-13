#ifndef SPLITGUI_LOGGER_CPP
#define SPLITGUI_LOGGER_CPP

#include <splitgui/logger.hpp>

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>

namespace SplitGui {
    Logger::Logger() {

    }

    Logger* Logger::instance = nullptr;

    void Logger::cleanup() {
        delete instance;
    }

    std::string Logger::getTime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        auto str = oss.str();
        
        return str;
    }

    Logger* Logger::getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void Logger::info(std::string message) {
        std::string time = getTime();
        fprintf(getInstance()->outputBuffer, "%s - INFO  - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        fflush(getInstance()->outputBuffer);
    }

    void Logger::debug(std::string message) {
        std::string time = getTime();
        fprintf(getInstance()->outputBuffer, "%s - DEBUG - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        fflush(getInstance()->outputBuffer);
    }

    void Logger::warn(std::string message) {
        std::string time = getTime();
        fprintf(getInstance()->outputBuffer, "%s - WARN  - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        fflush(getInstance()->outputBuffer);
    }

    void Logger::error(std::string message) {
        std::string time = getTime();
        fprintf(getInstance()->outputBuffer, "%s - ERROR - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        fflush(getInstance()->outputBuffer);
    }

    void Logger::fatal(std::string message) {
        std::string time = getTime();
        fprintf(getInstance()->outputBuffer, "%s - FATAL - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        fflush(getInstance()->outputBuffer);
    }

}

#endif