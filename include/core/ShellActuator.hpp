#ifndef DESKTOP_MANAGER_SHELLACTUATOR_HPP
#define DESKTOP_MANAGER_SHELLACTUATOR_HPP
#include <string>

#include "logging/include/logging/LogManager.hpp"
#include "logging/logger/Logger.hpp"


class ShellActuator
{
public:
    ShellActuator() = default;
    ~ShellActuator() = default;

    std::string executeShellCommand(const std::string& cmd) const;
    void printShellOutput(const std::string& output) const;

private:
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<ShellActuator>();
    std::pair<std::string, int> executeShellCommandStatus(const std::string& cmd) const;
};

typedef std::shared_ptr<ShellActuator> ShellActuatorHandle;


#endif //DESKTOP_MANAGER_SHELLACTUATOR_HPP