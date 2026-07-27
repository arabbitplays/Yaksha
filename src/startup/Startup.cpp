#include "include/startup/Startup.hpp"
#include <utility>

#include "workspaces/WorkspaceService.hpp"

Startup::Startup(KittyBindingHandle kitty_binding,
                 std::shared_ptr<WorkspaceService> workspace_service,
                 CommandExecutor executor)
    : kitty_binding(std::move(kitty_binding)),
      workspace_service(std::move(workspace_service)),
      execute(std::move(executor)) {}

void Startup::setupTheme() {
    execute("theme tokyo");
}

void Startup::setupWorkspaces()
{
    workspace_service->initExistingMonitors();
}

void Startup::runDashboardTerminal() {
    kitty_binding->launchDashboard();
}
