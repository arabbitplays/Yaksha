#include "app/DesktopManager.hpp"

#include "../../include/core/IController.hpp"
#include "../../include/syncing/SyncController.hpp"
#include "../../include/theming/ThemeController.hpp"
#include "../../include/workspaces/WorkspaceController.hpp"
#include "../../include/core/sockets/SocketServer.hpp"
#include "io/CommandParser.hpp"
#include "startup/Startup.hpp"
#include <memory>


DesktopManager::DesktopManager(bool dev_mode) : dev_mode(dev_mode)
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
}

void DesktopManager::initApp()
{
    shell_actuator = std::make_shared<ShellActuator>();

    hyprland_binding = std::make_shared<HyprlandBinding>(shell_actuator);
    swww_binding = std::make_shared<SwwwBinding>(shell_actuator);
    kitty_binding = std::make_shared<KittyBinding>(shell_actuator);
    waybar_binding = std::make_shared<WaybarBinding>(shell_actuator);
    git_binding = std::make_shared<GitBinding>(shell_actuator);

    workspace_service = std::make_shared<WorkspaceService>(hyprland_binding);
    theme_service = std::make_shared<ThemeService>(
        shell_actuator, hyprland_binding, swww_binding, kitty_binding, waybar_binding);
    sync_service = std::make_shared<SyncService>(shell_actuator, git_binding);

    registerController(std::make_shared<ThemeController>(theme_service));
    registerController(std::make_shared<WorkspaceController>(workspace_service));
    registerController(std::make_shared<SyncController>(sync_service));

    hypr_event_manager = std::make_unique<HyprEventManager>(workspace_service);

    if (!dev_mode)
    {
        initDesktopEnvironment();
    }
}

void DesktopManager::registerController(const std::shared_ptr<IController>& controller)
{
    controllers[controller->getKeyword()] = controller;
}

void DesktopManager::initDesktopEnvironment()
{
    LOGGER->info("Initialising Desktop Environment");
    Startup startup(kitty_binding, workspace_service, [this](const std::string& cmd) { return executeCommand(cmd); });
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

        hypr_event_manager->poll();
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
