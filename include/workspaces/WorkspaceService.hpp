#ifndef DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#define DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#include "bindings/HyprlandBinding.hpp"
#include "model/WindowMovement.hpp"
#include "model/Workspace.hpp"

class WorkspaceService
{
public:
    explicit WorkspaceService(HyprlandBindingHandle hyprland_binding);
    ~WorkspaceService() = default;

    void initMonitor(std::string monitor_name) const;
    void initExistingMonitors() const;
    void switchWorkspace(uint32_t target_virtual) const;
    void sendWindow(uint32_t target_virtual) const;
    void moveWindow(WindowMovement movement) const;
    Workspace getActiveWorkspace() const;
private:
    Workspace fromHyprlandId(uint32_t hyprland_id) const;
    static uint32_t toHyprlandId(const Workspace& workspace);

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<WorkspaceService>();

    HyprlandBindingHandle hyprland_binding;
    std::vector<std::string> monitor_names;
};


#endif //DESKTOP_MANAGER_WORKSPACESERVICE_HPP
