#include "bindings/SystemBinding.hpp"

#include <utility>

SystemBinding::SystemBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

std::string SystemBinding::currentDate() const {
    return shell_actuator->executeShellCommand("date").response;
}

void SystemBinding::copyFile(const std::string& src, const std::string& dst) const {
    shell_actuator->executeShellCommand("cp " + src + " " + dst);
}
