#ifndef DESKTOP_MANAGER_SHELLACTUATOR_HPP
#define DESKTOP_MANAGER_SHELLACTUATOR_HPP
#include <string>

#include "ShellResult.hpp"
#include <logging/LogManager.hpp>
#include "logging/logger/Logger.hpp"


class ShellActuator
{
public:
    ShellActuator() = default;
    ~ShellActuator() = default;

    ShellResult executeShellCommandRaw(const std::string& cmd) const;
    ShellResult executeShellCommand(const std::string& cmd) const;
    int32_t executeShellCommandSilent(const std::string& cmd) const;
    void printShellOutput(const std::string& output) const;

private:
    std::pair<std::string, int> executeShellCommandStatus(const std::string& cmd) const;
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<ShellActuator>();
};

typedef std::shared_ptr<ShellActuator> ShellActuatorHandle;


#endif //DESKTOP_MANAGER_SHELLACTUATOR_HPP