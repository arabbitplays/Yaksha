#include "../../include/app/HyprEventManager.hpp"

#include "core/util/StringUtil.h"
#include "workspaces/WorkspaceService.hpp"

HyprEventManager::HyprEventManager(const std::shared_ptr<WorkspaceService>& workspace_service) : socket_listener(getHyprSocketPath()), workspace_service(workspace_service)
{
    socket_listener.listen();
}

void HyprEventManager::poll()
{
    std::vector<std::string> messages = socket_listener.receiveMessages();
    for (const auto& message : messages)
    {
        HyprMessage hypr_message = parseHyprMessage(message);
        handleMessage(hypr_message);
    }
}

HyprEventManager::HyprMessage HyprEventManager::parseHyprMessage(const std::string& message)
{
    std::vector<std::string> parts = StringUtil::split(message, ">>");
    assert(parts.size() > 0);
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
        assert(hypr_message.arguments.size() > 0);
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
    return std::string(getenv("XDG_RUNTIME_DIR"))
        + "/hypr/"
        + getenv("HYPRLAND_INSTANCE_SIGNATURE")
        + "/.socket2.sock";
}
