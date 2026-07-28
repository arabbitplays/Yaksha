#include "../../include/workspaces/WorkspaceService.hpp"

#include <utility>

#include "workspaces/model/Monitor.hpp"
#include "workspaces/model/WindowMovement.hpp"

WorkspaceService::WorkspaceService(const MonitorStateHandle& monitor_state, HyprlandBindingHandle hyprland_binding)
    : monitor_state(monitor_state), hyprland_binding(std::move(hyprland_binding))
{
}

void WorkspaceService::initWorkspaceForMonitor(const MonitorHandle& monitor) const
{
    const uint32_t hyprland_id = toHyprlandId({monitor->physical_id, monitor_state->current_virtual_id});
    hyprland_binding->focusAndOpenWorkspaces({{monitor->name, hyprland_id}});
    logger->info("Initialized monitor " + monitor->name + " to id " + std::to_string(hyprland_id));
}

void WorkspaceService::switchWorkspace(uint32_t target_virtual) const
{
    Workspace active_workspace = getActiveWorkspace();
    std::vector<MonitorHandle> connected_monitors = monitor_state->getConnectedMonitors();

    std::vector<HyprlandBinding::MonitorWorkspace> focuses;
    focuses.reserve(connected_monitors.size());
    for (const auto& monitor : connected_monitors)
    {
        focuses.push_back({monitor->name, toHyprlandId({monitor->physical_id, target_virtual})});
    }
    hyprland_binding->
        focusAndOpenWorkspaces(focuses, monitor_state->getMonitorById(active_workspace.physical_id)->name);
    monitor_state->current_virtual_id = target_virtual;
}

void WorkspaceService::sendWindow(const uint32_t target_virtual) const
{
    std::string active_window = hyprland_binding->getActiveWindowAddress();
    Workspace workspace = getActiveWorkspace();
    uint32_t hyprland_id = toHyprlandId({workspace.physical_id, target_virtual});
    hyprland_binding->moveWindowToWorkspace(hyprland_id, active_window, /*silent=*/true);
}

void WorkspaceService::moveWindow(const WindowMovement movement) const
{
    std::string active_window = hyprland_binding->getActiveWindowAddress();
    Workspace active_workspace = getActiveWorkspace();
    MonitorHandle active_monitor = monitor_state->getMonitorById(active_workspace.physical_id);
    std::optional<MonitorHandle> adjacent_monitor = monitor_state->getAdjacentMonitor(active_monitor, movement);
    if (!adjacent_monitor.has_value())
    {
        logger->debug("No window to move to found from monitor " + active_monitor->name);
        return;
    }

    if (active_workspace.virtual_id != monitor_state->current_virtual_id)
    {
        logger->warn(std::format("Virtual id of active workspace {} does not match virtual id of the monitor state {}",
                                 active_workspace.virtual_id, monitor_state->current_virtual_id));
    }

    const uint32_t hyprland_id = toHyprlandId(
        {adjacent_monitor.value()->physical_id, monitor_state->current_virtual_id});
    hyprland_binding->moveWindowToWorkspace(hyprland_id, active_window, /*silent=*/false);
}

Workspace WorkspaceService::getActiveWorkspace() const
{
    return fromHyprlandId(hyprland_binding->getActiveWorkspaceId());
}

Workspace WorkspaceService::fromHyprlandId(uint32_t hyprland_id) const
{
    if (hyprland_id < 11)
    {
        throw std::runtime_error("Invalid hyprland workspace id " + std::to_string(hyprland_id));
    }
    Workspace workspace = {hyprland_id / 10 - 1, hyprland_id % 10 - 1};
    return workspace;
}

uint32_t WorkspaceService::toHyprlandId(const Workspace& workspace)
{
    return (workspace.physical_id + 1) * 10 + (workspace.virtual_id + 1);
}
