#include "../../include/app/HyprEventManager.h"

#include "core/util/StringUtil.h"

HyprEventManager::HyprEventManager() : socket_listener(getHyprSocketPath())
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

    logger->info(hypr_message.message_type);
    for (const auto& arg : hypr_message.arguments)
        logger->info("Arg: " + arg);
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
