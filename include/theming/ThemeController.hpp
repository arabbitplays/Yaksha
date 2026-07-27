#ifndef THEME_CONTROLLER
#define THEME_CONTROLLER

#include <memory>
#include <string>

#include "ThemeService.hpp"
#include "core/IController.hpp"
#include "io/CommandParser.hpp"

class ThemeController : public IController {
public:
    explicit ThemeController(std::shared_ptr<ThemeService> theme_service);
    ~ThemeController() override = default;

    [[nodiscard]] std::string getKeyword() const override;
    std::string execute(io::CommandHandle& cmd) override;

private:
    std::shared_ptr<ThemeService> theme_service;
};

#endif // THEME_CONTROLLER
