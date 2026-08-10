#include "include/startup/Startup.hpp"

#include <utility>

#include "dashboard/Dashboard.hpp"
#include "workspaces/MonitorService.hpp"

Startup::Startup(std::shared_ptr<Dashboard> dashboard,
                 std::shared_ptr<MonitorService> monitor_service,
                 CommandExecutor executor)
    : dashboard(std::move(dashboard)),
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
    dashboard->launch();
}
