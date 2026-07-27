#include "bindings/WaybarBinding.hpp"

#include <utility>

WaybarBinding::WaybarBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

void WaybarBinding::restart() const {
    shell_actuator->executeShellCommand("pkill waybar");
    shell_actuator->executeShellCommandRaw("waybar >/dev/null 2>&1 &");
}
