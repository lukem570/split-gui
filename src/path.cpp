#ifndef SPLITGUI_PATH
#define SPLITGUI_PATH

#include <splitgui/result.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace SplitGui {
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
    std::string getExecutablePath() {
        char buffer[MAX_PATH];

        for (int i = 0; i < MAX_PATH; i++) {
            buffer[i] = '\0';
        }
        
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        
        for (int i = MAX_PATH - 1; i >= 0; i--) {
            if (buffer[i] == '\\') {
                buffer[i + 1] = '\0';
                break;
            }
        }

        return std::string(buffer);
    }

#elif defined(__linux__) || defined(__unix__)
#include <unistd.h>
#include <limits.h>
    std::string getExecutablePath() {
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
        if (len != -1) {
            buffer[len] = '\0';
            return std::string(buffer);
        }
        return "";
    }

#elif defined(__APPLE__)
#include <mach-o/dyld.h>
    std::string getExecutablePath() { //untested
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            return std::string(buffer);
        }
        return "";
    }

#else
#error "Unknown platform"
#endif

    std::string remove_extension(const std::string& filePath) {
        size_t lastSlashPos = filePath.find_last_of("/\\");
        
        std::string parentFolder = filePath.substr(0, lastSlashPos);
        
        size_t dotPos = filePath.find_last_of(".");
        
        std::string fileWithoutExtension = filePath.substr(lastSlashPos + 1, dotPos - lastSlashPos - 1);

        return parentFolder;
    }

    ResultValue<std::vector<char>> readFile(const std::string& filename) {
        std::string path = remove_extension(getExecutablePath()) + '/' + filename;

        std::ifstream file;
        file.open(path, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            return Result::eFailedToOpenFile;
        }

        int fileSize = file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

};

#endif