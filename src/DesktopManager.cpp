#include "include/DesktopManager.hpp"
#include "../include/core/IController.hpp"
#include "../include/syncing/SyncController.hpp"
#include "../include/theming/ThemeController.hpp"
#include "../include/workspaces/WorkspaceController.hpp"
#include "io/CommandParser.hpp"
#include "startup/Startup.hpp"
#include <cmath>
#include <concepts>
#include <memory>

#include <cerrno>
#include <cstring>
#include <ostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <util/MonitorUtil.hpp>

#include "../include/core/sockets/SocketServer.hpp"

DesktopManager::DesktopManager(bool dev_mode)
{
    socket_path = dev_mode
                      ? "/tmp/desktop-manager-dev.sock"
                      : std::string(getenv("XDG_RUNTIME_DIR")) + "/desktop-manager/desktop-manager.sock";
    addController<ThemeController>();
    addController<WorkspaceController>();
    addController<SyncController>();

    if (!dev_mode)
    {
        initDesktopEnvironment();
    }
}

void DesktopManager::initDesktopEnvironment()
{
    LOGGER->info("Initialising Desktop Environment");
    Startup startup(shell_actuator, [this](const std::string& cmd) { return executeCommand(cmd); });
    startup.setupWorkspaces();
    startup.setupTheme();
    startup.runDashboardTerminal();
    LOGGER->info("Finished initialising Desktop Environment");
}

void DesktopManager::run()
{
    SocketServer server(socket_path);
    server.create();
    while (true)
    {
        int32_t client = server.acceptClient();
        std::string request = server.receive(client);
        if (!request.empty())
        {
            std::string response = executeCommand(request);
            server.send(client, response);
        }
        server.closeClient(client);
    }
}

std::string DesktopManager::executeCommand(const std::string& cmd_string) const
{
    LOGGER->info("Received command: " + cmd_string);

    try
    {
        io::CommandParser parser;
        io::CommandHandle cmd = parser.parseCommand(cmd_string);

        if (!controllers.contains(cmd->keyword))
            return "Error: Controller with keyword " + cmd->keyword + " does not exist";

        return controllers.at(cmd->keyword)->execute(cmd);
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error while executing command: " + std::string(e.what());
        // TODO why returning?
        return msg;
    }
}
