#include "include/startup/Startup.hpp"
#include <utility>

#include "core/ShellActuator.hpp"
#include "workspaces/WorkspaceService.hpp"

Startup::Startup(const ShellActuatorHandle& shell_actuator, const std::shared_ptr<WorkspaceService> workspace_service, CommandExecutor executor)
        : shell_actuator(shell_actuator), workspace_service(workspace_service), execute(std::move(executor)) {}

void Startup::setupTheme() {
    execute("theme tokyo");
}

void Startup::setupWorkspaces()
{
    workspace_service->initExistingMonitors();
}

void Startup::runDashboardTerminal() {
    // Detached so the daemon does not block on the terminal's lifetime.
    shell_actuator->executeShellCommandRaw(
        "setsid kitty --hold echo 'Hello Oschdi ^^' "
        ">/dev/null 2>&1 </dev/null &");
}
