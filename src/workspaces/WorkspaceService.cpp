#include "../../include/workspaces/WorkspaceService.hpp"

#include <utility>

#include "util/MonitorUtil.hpp"
#include "workspaces/model/WindowMovement.hpp"

WorkspaceService::WorkspaceService(HyprlandBindingHandle hyprland_binding)
    : hyprland_binding(std::move(hyprland_binding))
{
    monitor_names = MonitorUtil::getMonitorNamesForCurrSystem();
}

void WorkspaceService::initMonitor(std::string monitor_name) const {
    int32_t physical_id = -1;
    for (uint32_t i = 0; i < monitor_names.size(); i++)
    {
        if (monitor_names.at(i) == monitor_name)
        {
            physical_id = i;
        }
    }
    if (physical_id < 0)
    {
        logger->warn("Can not initialize monitor: Unknow monitor name " + monitor_name);
        return;
    }

    const uint32_t hyprland_id = toHyprlandId({static_cast<uint32_t>(physical_id), 0});
    hyprland_binding->focusAndOpenWorkspaces({{monitor_name, hyprland_id}});
}

void WorkspaceService::initExistingMonitors() const {
    std::vector<std::string> monitors;
    try
    {
        monitors = hyprland_binding->getMonitorNames();
    }
    catch (const std::exception& e)
    {
        logger->warn(std::string("Failed to query existing monitors: ") + e.what());
        return;
    }
    for (const std::string& name : monitors)
    {
        initMonitor(name);
    }
}

void WorkspaceService::switchWorkspace(uint32_t target_virtual) const {
    Workspace active_workspace = getActiveWorkspace();

    std::vector<HyprlandBinding::MonitorWorkspace> focuses;
    focuses.reserve(monitor_names.size());
    for (uint32_t i = 0; i < monitor_names.size(); i++) {
        focuses.push_back({monitor_names.at(i), toHyprlandId({i, target_virtual})});
    }
    hyprland_binding->focusAndOpenWorkspaces(focuses, monitor_names[active_workspace.physical_id]);
}

void WorkspaceService::sendWindow(const uint32_t target_virtual) const {
    std::string active_window = hyprland_binding->getActiveWindowAddress();
    Workspace workspace = getActiveWorkspace();
    uint32_t hyprland_id = toHyprlandId({workspace.physical_id, target_virtual});
    hyprland_binding->moveWindowToWorkspace(hyprland_id, active_window, /*silent=*/true);
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

    std::string active_window = hyprland_binding->getActiveWindowAddress();
    Workspace workspace = getActiveWorkspace();

    int32_t monitor_count = static_cast<int32_t>(monitor_names.size());
    int32_t physical_id = static_cast<int32_t>(workspace.physical_id) + physical_delta;
    physical_id = physical_id < 0 ? monitor_count - 1 : physical_id;
    physical_id = physical_id >= monitor_count ? 0 : physical_id;

    uint32_t hyprland_id = toHyprlandId({static_cast<uint32_t>(physical_id), workspace.virtual_id});
    hyprland_binding->moveWindowToWorkspace(hyprland_id, active_window, /*silent=*/false);
}

Workspace WorkspaceService::getActiveWorkspace() const {
    return fromHyprlandId(hyprland_binding->getActiveWorkspaceId());
}

Workspace WorkspaceService::fromHyprlandId(uint32_t hyprland_id) const {
    Workspace workspace = {hyprland_id / 10 - 1, hyprland_id % 10 - 1};
    if(workspace.physical_id >= monitor_names.size())
    {
        throw std::runtime_error("Invalid hyprland workspace id " + std::to_string(hyprland_id));
    }
    return workspace;
}

uint32_t WorkspaceService::toHyprlandId(const Workspace& workspace) {
    return (workspace.physical_id + 1) * 10 + (workspace.virtual_id + 1);
}
