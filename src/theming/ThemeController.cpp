#include "../../include/theming/ThemeController.hpp"
#include "io/CommandParser.hpp"
#include "util/MonitorUtil.hpp"
#include <filesystem>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

ThemeController::ThemeController(const ShellActuatorHandle& shell_actuator) {
    theme_service = std::make_shared<ThemeService>(shell_actuator);
}

std::string ThemeController::getKeyword() const {
    return "theme";
}

std::string ThemeController::execute(io::CommandHandle& cmd) {
    if (cmd->args.size() != 1) {
        throw std::runtime_error("Command 'theme' requires one string argument\n");
    };

    std::string theme_name = cmd->args[0];
    theme_service->setTheme(theme_name);
    return "ok";
}

