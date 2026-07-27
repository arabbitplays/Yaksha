#ifndef YAKSHA_HYPREVENTMANAGER_H
#define YAKSHA_HYPREVENTMANAGER_H
#include "core/sockets/SocketListener.hpp"


class HyprEventManager
{
    struct HyprMessage
    {
        std::string message_type;
        std::vector<std::string> arguments;
    };
public:
    HyprEventManager();
    ~HyprEventManager() = default;

    void poll();

private:
    void handleMessage(const HyprMessage& hypr_message) const;
    bool ignoreMessage(const HyprMessage& hypr_message) const;
    HyprMessage parseHyprMessage(const std::string& message);

    static std::string getHyprSocketPath();

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<HyprEventManager>();
    SocketListener socket_listener;

    std::vector<std::string> handled_message_types = { "monitoradded", "monitorremoved" };
};


#endif //YAKSHA_HYPREVENTMANAGER_H