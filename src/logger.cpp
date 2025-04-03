#ifndef SPLITGUI_LOGGER_CPP
#define SPLITGUI_LOGGER_CPP

#include <splitgui/logger.hpp>
#include <splitgui/lib.hpp>

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>

namespace SplitGui {
    
#ifdef _WIN32
    
    bool supportsAnsi() {
        SPLITGUI_PROFILE;
        
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        
        if (hStdOut == INVALID_HANDLE_VALUE) {
            return false;
        }
        
        if (!GetConsoleMode(hStdOut, &dwMode)) {
            return false;
        }
        
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hStdOut, dwMode)) {
            return false;
        }
        
        return true;
    }
    
#elif defined(__linux__) || defined(__unix__)
    
    bool supportsAnsi() {
        SPLITGUI_PROFILE;
        
        const char *term = getenv("TERM");
        
        if (term != NULL) {
            if (strstr(term, "xterm") != NULL || 
            strstr(term, "screen") != NULL ||
            strstr(term, "linux") != NULL ||
            strstr(term, "vt100") != NULL) {
                return true;
            }
        }
        
        return false;
    }
    
#endif
    
    Logger* Logger::instance = nullptr;
    
    Logger::Logger() {
        ansiAware = supportsAnsi();
    }
    
    void Logger::cleanup() {
        SPLITGUI_PROFILE;
        
        delete instance;
    }

    std::string Logger::getTime() {
        SPLITGUI_PROFILE;

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        auto str = oss.str();
        
        return str;
    }

    Logger* Logger::getInstance() {
        SPLITGUI_PROFILE;

        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void Logger::info(std::string message) {
        SPLITGUI_PROFILE;

        std::string time = getTime();

        if (getInstance()->ansiAware) {
            fprintf(getInstance()->outputBuffer, "\033[0;32m%s\033[0m - \033[0;36mINFO \033[0m - \033[0;34m[SPLITGUI]\033[0m %s\n", time.c_str(), message.c_str());
        } else {
            fprintf(getInstance()->outputBuffer, "%s - INFO  - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        }

        fflush(getInstance()->outputBuffer);

        SPLITGUI_PROFILE_LOG(message.c_str(), message.size());
    }

    void Logger::debug(std::string message) {
        SPLITGUI_PROFILE;

        std::string time = getTime();

        if (getInstance()->ansiAware) {
            fprintf(getInstance()->outputBuffer, "\033[0;32m%s\033[0m - \033[0;35mDEBUG\033[0m - \033[0;34m[SPLITGUI]\033[0m %s\n", time.c_str(), message.c_str());
        } else {
            fprintf(getInstance()->outputBuffer, "%s - DEBUG - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        }
        
        fflush(getInstance()->outputBuffer);

        SPLITGUI_PROFILE_LOG(message.c_str(), message.size());
    }

    void Logger::warn(std::string message) {
        SPLITGUI_PROFILE;

        std::string time = getTime();

        if (getInstance()->ansiAware) {
            fprintf(getInstance()->outputBuffer, "\033[0;32m%s\033[0m - \033[0;33mWARN \033[0m - \033[0;34m[SPLITGUI]\033[0m %s\n", time.c_str(), message.c_str());
        } else {
            fprintf(getInstance()->outputBuffer, "%s - WARN  - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        }
        
        fflush(getInstance()->outputBuffer);

        SPLITGUI_PROFILE_LOG(message.c_str(), message.size());
    }

    void Logger::error(std::string message) {
        SPLITGUI_PROFILE;

        std::string time = getTime();

        if (getInstance()->ansiAware) {
            fprintf(getInstance()->outputBuffer, "\033[0;32m%s\033[0m - \033[0;31mERROR\033[0m - \033[0;34m[SPLITGUI]\033[0m %s\n", time.c_str(), message.c_str());
        } else {
            fprintf(getInstance()->outputBuffer, "%s - ERROR - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        }
        
        fflush(getInstance()->outputBuffer);

        SPLITGUI_PROFILE_LOG(message.c_str(), message.size());
    }

    void Logger::fatal(std::string message) {
        SPLITGUI_PROFILE;

        std::string time = getTime();

        if (getInstance()->ansiAware) {
            fprintf(getInstance()->outputBuffer, "\033[0;32m%s\033[0m - \033[1;31mFATAL\033[0m - \033[0;34m[SPLITGUI]\033[0m %s\n", time.c_str(), message.c_str());
        } else {
            fprintf(getInstance()->outputBuffer, "%s - FATAL - [SPLITGUI] %s\n", time.c_str(), message.c_str());
        }
        
        fflush(getInstance()->outputBuffer);

        SPLITGUI_PROFILE_LOG(message.c_str(), message.size());
    }

}

#endif