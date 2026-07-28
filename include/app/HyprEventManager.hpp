#ifndef YAKSHA_HYPREVENTMANAGER_H
#define YAKSHA_HYPREVENTMANAGER_H
#include "core/sockets/SocketListener.hpp"
#include "workspaces/MonitorService.hpp"
#include "workspaces/WorkspaceService.hpp"


class HyprEventManager
{
    struct HyprMessage
    {
        std::string message_type;
        std::vector<std::string> arguments;
    };
public:
    HyprEventManager(const std::shared_ptr<MonitorService>& monitor_service);
    ~HyprEventManager() = default;

    void poll();

private:
    void handleMessage(const HyprMessage& hypr_message) const;
    bool ignoreMessage(const HyprMessage& hypr_message) const;
    HyprMessage parseHyprMessage(const std::string& message);

    static std::string getHyprSocketPath();

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<HyprEventManager>();
    SocketListener socket_listener;

    std::shared_ptr<MonitorService> monitor_service;

    std::vector<std::string> handled_message_types = { "monitoradded", "monitorremoved" };
};


#endif //YAKSHA_HYPREVENTMANAGER_H