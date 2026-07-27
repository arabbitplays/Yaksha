#ifndef DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#define DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#include "core/ShellActuator.hpp"
#include "model/WindowMovement.hpp"
#include "model/Workspace.hpp"

class WorkspaceService
{
public:
    explicit WorkspaceService(const ShellActuatorHandle& shell_actuator);
    ~WorkspaceService() = default;

    void initMonitor(std::string monitor_name) const;
    void initExistingMonitors() const;
    void switchWorkspace(uint32_t target_virtual) const;
    void sendWindow(uint32_t target_virtual) const;
    void moveWindow(WindowMovement movement) const;
    Workspace getActiveWorkspace() const;
private:
    std::string getActiveWindowId() const;

    Workspace fromHyprlandId(uint32_t hyprland_id) const;
    static uint32_t toHyprlandId(const Workspace& workspace);

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<WorkspaceService>();

    ShellActuatorHandle shell_actuator;
    std::vector<std::string> monitor_names;
};


#endif //DESKTOP_MANAGER_WORKSPACESERVICE_HPP
