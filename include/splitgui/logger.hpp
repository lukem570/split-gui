#ifndef SPLITGUI_LOGGER_HPP
#define SPLITGUI_LOGGER_HPP

#include <string>
#include <chrono>

namespace SplitGui {
    enum class LogLevel {
        eDebug,
        eInfo,
        eWarn,
        eError,
        eFatal,
    };

    class Logger {
        public:
            static void cleanup();

            static void info(std::string message);
            static void debug(std::string message);
            static void warn(std::string message);
            static void error(std::string message);
            static void fatal(std::string message);

        private:
            static Logger* instance;
            
            FILE* outputBuffer = stdout;

            Logger();

            static std::string getTime();
            static Logger* getInstance();
    };
}

#endif