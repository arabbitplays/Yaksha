#ifndef STARTUP
#define STARTUP

#include <functional>
#include <memory>
#include <string>

#include "bindings/KittyBinding.hpp"
#include "dashboard/Dashboard.hpp"
#include "workspaces/MonitorService.hpp"
#include "workspaces/WorkspaceService.hpp"

class Startup
{
public:
    using CommandExecutor = std::function<std::string(const std::string&)>;

    Startup(KittyBindingHandle kitty_binding, std::shared_ptr<MonitorService> monitor_service,
            CommandExecutor executor);
    ~Startup() = default;

    void setupWorkspaces();
    void setupTheme();
    void runDashboardTerminal();

private:
    KittyBindingHandle kitty_binding;
    std::shared_ptr<MonitorService> monitor_service;
    CommandExecutor execute;

    std::shared_ptr<Dashboard> dashboard;
};

#endif // STARTUP
