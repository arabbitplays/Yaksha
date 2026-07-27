#ifndef STARTUP
#define STARTUP

#include <functional>
#include <string>

#include "core/ShellActuator.hpp"
#include "workspaces/WorkspaceService.hpp"

class Startup {
public:
    using CommandExecutor = std::function<std::string(const std::string&)>;

    Startup(const ShellActuatorHandle& shell_actuator, std::shared_ptr<WorkspaceService> workspace_service,
            CommandExecutor executor);
    ~Startup() = default;

    void setupWorkspaces();
    void setupTheme();
    void runDashboardTerminal();

private:
    ShellActuatorHandle shell_actuator;
    std::shared_ptr<WorkspaceService> workspace_service;
    CommandExecutor execute;
};

#endif // STARTUP
