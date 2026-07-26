#ifndef THEME_CONTROLLER
#define THEME_CONTROLLER

#include <string>
#include "io/CommandParser.hpp"

#include "ThemeService.hpp"
#include "core/IController.hpp"
#include "core/ShellActuator.hpp"

class ThemeController : public IController {
public:

    explicit ThemeController(const ShellActuatorHandle& shell_actuator);
    ~ThemeController() override = default;

    [[nodiscard]] std::string getKeyword() const override;
    std::string execute(io::CommandHandle& cmd) override;

private:
    std::shared_ptr<ThemeService> theme_service;
};

#endif // THEME_CONTROLLER

