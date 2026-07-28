#ifndef YAKSHA_MONITORSERVICE_HPP
#define YAKSHA_MONITORSERVICE_HPP
#include "WorkspaceService.hpp"
#include "model/MonitorState.hpp"


class MonitorService
{
public:
    MonitorService(const MonitorStateHandle& monitor_state, const std::shared_ptr<WorkspaceService>& workspace_service,
                   const std::shared_ptr<HyprlandBinding>& hyprland_binding);

    void addMonitor(const std::string& name);
    void removeMonitor(const std::string& name);
    void addAlreadyConnectedMonitors();
private:
    void initMonitor(const MonitorHandle& monitor_handle);
    uint32_t physicalIdOf(const std::string& name);

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<MonitorService>();

    MonitorStateHandle monitor_state;
    std::shared_ptr<WorkspaceService> workspace_service;
    std::shared_ptr<HyprlandBinding> hyprland_binding;

    std::unordered_map<std::string, uint32_t> physicalIds;
    uint32_t next_physicalId = 0;
};


#endif //YAKSHA_MONITORSERVICE_HPP
