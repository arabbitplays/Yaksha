#ifndef DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#define DESKTOP_MANAGER_WORKSPACESERVICE_HPP
#include "bindings/HyprlandBinding.hpp"
#include "model/Monitor.hpp"
#include "model/MonitorState.hpp"
#include "model/WindowMovement.hpp"
#include "model/Workspace.hpp"

class WorkspaceService
{
public:
    explicit WorkspaceService(const MonitorStateHandle& monitor_state, HyprlandBindingHandle hyprland_binding);
    ~WorkspaceService() = default;

    void initWorkspaceForMonitor(const MonitorHandle& monitor) const;
    void switchWorkspace(uint32_t target_virtual) const;
    void sendWindow(uint32_t target_virtual) const;
    void moveWindow(WindowMovement movement) const;
    Workspace getActiveWorkspace() const;
private:
    Workspace fromHyprlandId(uint32_t hyprland_id) const;
    static uint32_t toHyprlandId(const Workspace& workspace);

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<WorkspaceService>();

    MonitorStateHandle monitor_state;
    HyprlandBindingHandle hyprland_binding;
};


#endif //DESKTOP_MANAGER_WORKSPACESERVICE_HPP
