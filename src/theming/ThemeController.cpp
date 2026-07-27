#include "../../include/theming/ThemeController.hpp"
#include "io/CommandParser.hpp"
#include <stdexcept>
#include <string>
#include <utility>

ThemeController::ThemeController(std::shared_ptr<ThemeService> theme_service)
    : theme_service(std::move(theme_service)) {}

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
