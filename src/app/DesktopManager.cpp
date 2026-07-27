#include "app/DesktopManager.hpp"

#include "../../include/core/IController.hpp"
#include "../../include/syncing/SyncController.hpp"
#include "../../include/theming/ThemeController.hpp"
#include "../../include/workspaces/WorkspaceController.hpp"
#include "../../include/core/sockets/SocketServer.hpp"
#include "../../include/core/sockets/SocketListener.hpp"
#include "io/CommandParser.hpp"
#include "startup/Startup.hpp"
#include <cmath>
#include <memory>

#include <util/MonitorUtil.hpp>


DesktopManager::DesktopManager(bool dev_mode)
{
    if (dev_mode)
    {
        socket_path = "/tmp/desktop-manager-dev.sock";
    }
    else
    {
        const char* runtime_dir = getenv("XDG_RUNTIME_DIR");
        if (runtime_dir == nullptr)
        {
            throw std::runtime_error("XDG_RUNTIME_DIR is not set");
        }
        socket_path = std::string(runtime_dir) + "/desktop-manager/desktop-manager.sock";
    }
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
    Startup startup(shell_actuator, workspace_service, [this](const std::string& cmd) { return executeCommand(cmd); });
    startup.setupTheme();
    startup.setupWorkspaces();
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

        hypr_event_manager.poll();
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

        try
        {
            return controllers.at(cmd->keyword)->execute(cmd);
        } catch (std::exception& e)
        {
            LOGGER->error(e.what());
            return "Error: " + std::string(e.what());
        }
    }
    catch (const std::exception& e)
    {
        return "Error while executing command: " + std::string(e.what());
    }
}
