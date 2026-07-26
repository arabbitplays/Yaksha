#include "include/startup/Startup.hpp"
#include <utility>

#include "core/ShellActuator.hpp"

Startup::Startup(ShellActuatorHandle shell_actuator, CommandExecutor executor) : shell_actuator(shell_actuator), execute(std::move(executor)) {}

void Startup::setupWorkspaces() {
    execute("workspace switch 1");
}

void Startup::setupTheme() {
    execute("theme tokyo");
}

void Startup::runDashboardTerminal() {
    // Detached so the daemon does not block on the terminal's lifetime.
    shell_actuator->executeShellCommandRaw(
        "setsid kitty --hold echo 'Hello Oschdi ^^' "
        ">/dev/null 2>&1 </dev/null &");
}
