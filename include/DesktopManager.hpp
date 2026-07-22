#ifndef DESKTOP_MANAGER
#define DESKTOP_MANAGER

#include <concepts>
#include <memory>
#include <controller/IController.hpp>
#include <string>
#include <unordered_map>

#include "logging/include/logging/LogManager.hpp"
#include "logging/logger/Logger.hpp"

namespace foofoofoofoofoofoo
{
    namespace bar
    {
        struct Lol
        {

        };
    }
}

class DesktopManager {
public:
    explicit DesktopManager(bool dev_mode);
    ~DesktopManager() = default;

    void run();
private:
    template<std::derived_from<IController> T>
    void addController() {
        std::shared_ptr<IController> controller = std::make_shared<T>();
        controllers[controller->getKeyword()] = controller;
    }
    void initDesktopEnvironment();

    Logging::LoggerHandle LOGGER = Logging::LogManager::getClassLogger<foofoofoofoofoofoo::bar::Lol>();

    std::string executeCommand(const std::string& cmd_string) const;

    std::string socket_path;
    std::unordered_map<std::string, std::shared_ptr<IController>> controllers;
};

#endif // DESKTOP_MANAGER

