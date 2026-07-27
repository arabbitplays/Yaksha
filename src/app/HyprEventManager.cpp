#include "../../include/app/HyprEventManager.hpp"

#include "core/sockets/SocketException.hpp"
#include "core/util/StringUtil.h"
#include "workspaces/WorkspaceService.hpp"

HyprEventManager::HyprEventManager(const std::shared_ptr<WorkspaceService>& workspace_service) : socket_listener(getHyprSocketPath()), workspace_service(workspace_service)
{
    socket_listener.listen();
}

void HyprEventManager::poll()
{
    try
    {
        std::vector<std::string> messages = socket_listener.receiveMessages();
        for (const auto& message : messages)
        {
            HyprMessage hypr_message = parseHyprMessage(message);
            handleMessage(hypr_message);
        }
    }
    catch (const SocketException& e)
    {
        logger->error(std::string("Hypr socket error: ") + e.what());
    }
}

HyprEventManager::HyprMessage HyprEventManager::parseHyprMessage(const std::string& message)
{
    std::vector<std::string> parts = StringUtil::split(message, ">>");
    if (parts.empty())
    {
        throw SocketException("Malformed hypr message (no parts): " + message);
    }
    const std::vector args(parts.begin() + 1, parts.end());
    return {parts.at(0), args};
}

void HyprEventManager::handleMessage(const HyprMessage& hypr_message) const
{
    if (ignoreMessage(hypr_message))
    {
        return;
    }

    if (hypr_message.message_type == "monitoradded")
    {
        if (hypr_message.arguments.empty())
        {
            throw SocketException("monitoradded event without monitor name argument");
        }
        workspace_service->initMonitor(hypr_message.arguments.at(0));
    }

    std::string log_message = hypr_message.message_type;
    for (const auto& arg : hypr_message.arguments)
        log_message += " " + arg;
    logger->info(log_message);
}

bool HyprEventManager::ignoreMessage(const HyprMessage& hypr_message) const
{
    for (const auto& handled_type : handled_message_types)
    {
        if (handled_type == hypr_message.message_type)
        {
            return false;
        }
    }
    return true;
}

std::string HyprEventManager::getHyprSocketPath()
{
    const char* runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (runtime_dir == nullptr)
    {
        throw std::runtime_error("XDG_RUNTIME_DIR is not set");
    }
    const char* hypr_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (hypr_signature == nullptr)
    {
        throw std::runtime_error("HYPRLAND_INSTANCE_SIGNATURE is not set");
    }
    return std::string(runtime_dir) + "/hypr/" + hypr_signature + "/.socket2.sock";
}
