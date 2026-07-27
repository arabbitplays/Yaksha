#ifndef DESKTOP_MANAGER
#define DESKTOP_MANAGER

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>

#include "../core/IController.hpp"
#include "../core/ShellActuator.hpp"
#include <logging/LogManager.hpp>

#include "HyprEventManager.h"
#include "logging/logger/Logger.hpp"

class DesktopManager {
public:
    explicit DesktopManager(bool dev_mode);
    ~DesktopManager() = default;

    void run();
private:
    template<std::derived_from<IController> T>
    void addController() {
        std::shared_ptr<IController> controller = std::make_shared<T>(shell_actuator);
        controllers[controller->getKeyword()] = controller;
    }
    void initDesktopEnvironment();

    Logging::LoggerHandle LOGGER = Logging::LogManager::getClassLogger<DesktopManager>();

    std::string executeCommand(const std::string& cmd_string) const;

    HyprEventManager hypr_event_manager;

    ShellActuatorHandle shell_actuator = std::make_shared<ShellActuator>();
    std::string socket_path;
    std::unordered_map<std::string, std::shared_ptr<IController>> controllers;
};

#endif // DESKTOP_MANAGER

