#ifndef FILE_UTIL
#define FILE_UTIL

#include <fstream>

class FileUtil {
public:
    FileUtil() = default;
    ~FileUtil() = default;

    static std::string expandHome(const std::string& path) {
        if (!path.starts_with("~"))
            return path;

        const char* home = std::getenv("HOME");
        if (!home)
            throw std::runtime_error("HOME not set");

        return std::string(home) + path.substr(1);
    }

    static void overwriteFile(const std::string& path, const std::string& content) {
        std::ofstream file(expandHome(path), std::ios::out | std::ios::trunc);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        file << content;
        file.close();
    }
};

#endif // FILE_UTIL
