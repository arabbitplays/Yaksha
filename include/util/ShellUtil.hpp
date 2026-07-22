#ifndef SHELL_UTIL
#define SHELL_UTIL

#include <array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <utility>
class ShellUtil {
public:
    ShellUtil() = default;
    ~ShellUtil() = default;

    static std::string executeShellCommand(const std::string& cmd) {
        return executeShellCommandStatus(cmd).first;
    }

    // Returns {captured stdout+stderr chunk, exit status}. Exit status is -1
    // when the command could not be launched or terminated abnormally.
    static std::pair<std::string, int> executeShellCommandStatus(const std::string& cmd) {
        std::cout << "Executing shell command '" << cmd << "'" << std::endl;

        std::array<char, 255> buffer{};
        std::string result;

        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            return {"", -1};
        }

        while (fgets(buffer.data(), buffer.size(), pipe)) {
            result += buffer.data();
        }

        int raw = pclose(pipe);
        int status = (raw != -1 && WIFEXITED(raw)) ? WEXITSTATUS(raw) : -1;

        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return {result, status};
    }

    static void printShellOutput(const std::string& output) {
        std::cout << output << std::endl;
    }


    static int32_t parseStringToInt(const std::string& arg) {
        int32_t value;
        auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), value);
        
        if (ec == std::errc{} && ptr == arg.data() + arg.size()) {
            return value;
        } else {
            throw std::runtime_error("Error while parsing string to int");
        }
    }
private:
};

#endif // SHELL_UTIL

