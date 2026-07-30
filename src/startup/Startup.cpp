#include "include/startup/Startup.hpp"

#include <utility>

#include "dashboard/Dashboard.hpp"
#include "workspaces/MonitorService.hpp"

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
    dashboard = std::make_shared<Dashboard>(kitty_binding);
    dashboard->launch();
}
