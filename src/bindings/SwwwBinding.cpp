#include "bindings/SwwwBinding.hpp"

#include <utility>

SwwwBinding::SwwwBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

void SwwwBinding::setWallpaper(const std::string& path,
                               const std::optional<std::string>& monitor_name) const {
    std::string cmd = "swww img " + path + " " + TRANSITION_OPTIONS;
    if (monitor_name.has_value()) {
        cmd += " -o " + *monitor_name;
    }
    shell_actuator->executeShellCommand(cmd);
}
