#include "../../include/workspaces/WorkspaceService.hpp"

#include "util/MonitorUtil.hpp"
#include "workspaces/model/WindowMovement.hpp"

WorkspaceService::WorkspaceService(const ShellActuatorHandle& shell_actuator) : shell_actuator(shell_actuator)
{
    monitor_names = MonitorUtil::getMonitorNamesForCurrSystem();
}

void WorkspaceService::switchWorkspace(uint32_t target_virtual) const {
    Workspace active_workspace = getActiveWorkspace();

    std::string batch = "hyprctl --batch \"";
    for (uint32_t i = 0; i < monitor_names.size(); i++) {
        batch += "dispatch focusmonitor " + monitor_names[i] + " ; ";
        batch += "dispatch workspace " + std::to_string(toHyprlandId({i, target_virtual})) + " ; ";
    }
    batch += "dispatch focusmonitor " + monitor_names[active_workspace.physical_id] + "\"";

    shell_actuator->executeShellCommand(batch);
}

void WorkspaceService::sendWindow(const uint32_t target_virtual) const {
    std::string active_window = getActiveWindowId();
    Workspace workspace = getActiveWorkspace();
    uint32_t hyprland_id = toHyprlandId({workspace.physical_id, target_virtual});
    shell_actuator->executeShellCommand("hyprctl dispatch movetoworkspacesilent " + std::to_string(hyprland_id) + ",address:" + active_window);
}

void WorkspaceService::moveWindow(WindowMovement movement) const {
    int32_t physical_delta = 0;
    switch (movement)
    {
        case WindowMovement::LEFT:
            physical_delta = -1;
            break;
        case WindowMovement::RIGHT:
            physical_delta = 1;
            break;
    default:
            throw std::invalid_argument("Unknown movement " + std::to_string(movement));
    }

    std::string active_window = getActiveWindowId();
    Workspace workspace = getActiveWorkspace();

    int32_t monitor_count = static_cast<int32_t>(monitor_names.size());
    int32_t physical_id = static_cast<int32_t>(workspace.physical_id) + physical_delta;
    physical_id = physical_id < 0 ? monitor_count - 1 : physical_id;
    physical_id = physical_id >= monitor_count ? 0 : physical_id;

    uint32_t hyprland_id = toHyprlandId({static_cast<uint32_t>(physical_id), workspace.virtual_id});
    shell_actuator->executeShellCommand("hyprctl dispatch movetoworkspace " + std::to_string(hyprland_id) + ",address:" + active_window);
}

Workspace WorkspaceService::getActiveWorkspace() const {
    std::string active_workspace_string = shell_actuator->executeShellCommand("hyprctl activeworkspace -j | jq -r '.id'").response;
    uint32_t hyprland_id = std::stoi(active_workspace_string);
    return fromHyprlandId(hyprland_id);
}

std::string WorkspaceService::getActiveWindowId() const {
    return shell_actuator->executeShellCommand("hyprctl activewindow -j | jq -r '.address'").response;
}

Workspace WorkspaceService::fromHyprlandId(uint32_t hyprland_id) {
    return {hyprland_id / 10 - 1, hyprland_id % 10 - 1};
}

uint32_t WorkspaceService::toHyprlandId(const Workspace& workspace) {
    return (workspace.physical_id + 1) * 10 + (workspace.virtual_id + 1);
}
