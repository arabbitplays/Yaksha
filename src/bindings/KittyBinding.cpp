#include "bindings/KittyBinding.hpp"

#include <utility>

KittyBinding::KittyBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

void KittyBinding::reload() const {
    shell_actuator->executeShellCommand("kill -USR1 $(pidof kitty)");
}

void KittyBinding::launchDashboard() const {
    // Detached so the daemon does not block on the terminal's lifetime.
    shell_actuator->executeShellCommandRaw(
        "setsid kitty --hold echo 'Hello Oschdi ^^' "
        ">/dev/null 2>&1 </dev/null &");
}
