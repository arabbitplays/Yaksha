#ifndef THEME_CONTROLLER
#define THEME_CONTROLLER

#include <string>
#include "io/CommandParser.hpp"

#include "ThemeService.hpp"
#include "core/IController.hpp"

class ThemeController : public IController {
public:

    ThemeController();
    ~ThemeController() override = default;

    [[nodiscard]] std::string getKeyword() const override;
    std::string execute(io::CommandHandle& cmd) override;

private:
    std::shared_ptr<ThemeService> theme_service;
};

#endif // THEME_CONTROLLER

