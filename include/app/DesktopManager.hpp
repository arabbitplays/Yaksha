#ifndef DESKTOP_MANAGER
#define DESKTOP_MANAGER

#include <memory>
#include <string>
#include <unordered_map>

#include "../core/IController.hpp"
#include "../core/ShellActuator.hpp"
#include <logging/LogManager.hpp>

#include "HyprEventManager.hpp"
#include "logging/logger/Logger.hpp"
#include "syncing/SyncService.hpp"
#include "theming/ThemeService.hpp"
#include "workspaces/WorkspaceService.hpp"

class DesktopManager {
public:
    explicit DesktopManager(bool dev_mode);
    ~DesktopManager() = default;

    void initApp();
    void run();
private:
    void registerController(const std::shared_ptr<IController>& controller);
    void initDesktopEnvironment();

    Logging::LoggerHandle LOGGER = Logging::LogManager::getClassLogger<DesktopManager>();

    std::string executeCommand(const std::string& cmd_string) const;

    bool dev_mode;
    std::string socket_path;

    ShellActuatorHandle shell_actuator;

    std::shared_ptr<WorkspaceService> workspace_service;
    std::shared_ptr<ThemeService> theme_service;
    std::shared_ptr<SyncService> sync_service;

    std::unique_ptr<HyprEventManager> hypr_event_manager;

    std::unordered_map<std::string, std::shared_ptr<IController>> controllers;
};

#endif // DESKTOP_MANAGER
