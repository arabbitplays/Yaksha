#ifndef STARTUP
#define STARTUP

#include <functional>
#include <string>

class Startup {
public:
    using CommandExecutor = std::function<std::string(const std::string&)>;

    explicit Startup(CommandExecutor executor);
    ~Startup() = default;

    void setupWorkspaces();
    void setupTheme();
    void runDashboardTerminal();

private:
    CommandExecutor execute;
};

#endif // STARTUP
