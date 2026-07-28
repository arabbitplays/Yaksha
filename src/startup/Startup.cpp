#include "include/startup/Startup.hpp"
#include <utility>

#include "workspaces/MonitorService.hpp"
#include "workspaces/WorkspaceService.hpp"

Startup::Startup(KittyBindingHandle kitty_binding,
                 std::shared_ptr<MonitorService> monitor_service,
                 CommandExecutor executor)
    : kitty_binding(std::move(kitty_binding)),
      monitor_service(std::move(monitor_service)),
      execute(std::move(executor)) {}

void Startup::setupTheme() {
    execute("theme tokyo");
}

void Startup::setupWorkspaces()
{
    monitor_service->addAlreadyConnectedMonitors();
}

void Startup::runDashboardTerminal() {
    kitty_binding->launchDashboard();
}
