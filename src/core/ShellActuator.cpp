#include "include/core/ShellActuator.hpp"

#include "core/ShellResult.hpp"
#include "logging/logger/Logger.hpp"

ShellResult ShellActuator::executeShellCommand(const std::string& cmd) const
{
    // redirect stderr into stdout
    auto result = executeShellCommandStatus(cmd + " 2>&1");
    logger->trace("Status " + std::to_string(result.second) + " - " + result.first);
    return {result.first, result.second};
}

int32_t ShellActuator::executeShellCommandSilent(const std::string& cmd) const
{
    auto result = executeShellCommandStatus(cmd + " &>/dev/null 2>&1");
    logger->trace("Status: " + std::to_string(result.second));
    return result.second;
}

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