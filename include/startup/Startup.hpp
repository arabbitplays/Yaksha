#ifndef STARTUP
#define STARTUP

#include <functional>
#include <string>

#include "core/ShellActuator.hpp"

class Startup {
public:
    using CommandExecutor = std::function<std::string(const std::string&)>;

    explicit Startup(ShellActuatorHandle shell_actuator, CommandExecutor executor);
    ~Startup() = default;

    void setupWorkspaces();
    void setupTheme();
    void runDashboardTerminal();

private:
    ShellActuatorHandle shell_actuator;
    CommandExecutor execute;
};

#endif // STARTUP
