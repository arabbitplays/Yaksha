#include "../../include/workspaces/MonitorService.hpp"

MonitorService::MonitorService(const MonitorStateHandle& monitor_state,
                               const std::shared_ptr<WorkspaceService>& workspace_service,
                               const std::shared_ptr<HyprlandBinding>& hyprland_binding)
    : monitor_state(monitor_state), workspace_service(workspace_service), hyprland_binding(hyprland_binding)
{
}

void MonitorService::addMonitor(const std::string& name)
{
    if (monitor_state->monitors.contains(name))
    {
        logger->warn("Cannot add monitor " + name + " that is already connected");
        return;
    }

    MonitorHandle monitor = std::make_shared<Monitor>(physicalIdOf(name), name);
    monitor_state->monitors[name] = monitor;
    monitor_state->monitor_layout.tryAddingMonitorToRing(name);
    initMonitor(monitor);
}

void MonitorService::removeMonitor(const std::string& name)
{
    if (monitor_state->monitors.contains(name))
    {
        monitor_state->monitors.erase(name);
    }
    else
    {
        logger->warn("Trying to remove monitor " + name + " that is not connected");
    }
}

void MonitorService::addAlreadyConnectedMonitors()
{
    std::vector<std::string> connected_names = hyprland_binding->getMonitorNames();
    for (const std::string& name : connected_names)
    {
        addMonitor(name);
    }
}

void MonitorService::initMonitor(const MonitorHandle& monitor)
{
    workspace_service->initWorkspaceForMonitor(monitor);
}

uint32_t MonitorService::physicalIdOf(const std::string& name)
{
    if (!physicalIds.contains(name))
    {
        physicalIds[name] = next_physicalId++;
    }
    return physicalIds[name];
}
