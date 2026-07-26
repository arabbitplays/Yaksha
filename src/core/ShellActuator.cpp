#include "include/core/ShellActuator.hpp"

#include "logging/logger/Logger.hpp"

std::string ShellActuator::executeShellCommand(const std::string& cmd) const
{
    return executeShellCommandStatus(cmd).first;
}

// Returns {captured stdout+stderr chunk, exit status}. Exit status is -1
// when the command could not be launched or terminated abnormally.
std::pair<std::string, int> ShellActuator::executeShellCommandStatus(const std::string& cmd) const
{
    logger->debug("Executing shell command '" + cmd + "'");

    std::array<char, 255> buffer{};
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        return {"", -1};
    }

    while (fgets(buffer.data(), buffer.size(), pipe))
    {
        result += buffer.data();
    }

    int raw = pclose(pipe);
    int status = (raw != -1 && WIFEXITED(raw)) ? WEXITSTATUS(raw) : -1;

    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }

    return {result, status};
}

void ShellActuator::printShellOutput(const std::string& output) const
{
    logger->debug("Shell output: '" + output + "'");
}