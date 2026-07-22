#include "include/startup/Startup.hpp"
#include "util/ShellUtil.hpp"
#include <utility>

Startup::Startup(CommandExecutor executor) : execute(std::move(executor)) {}

void Startup::setupWorkspaces() {
    execute("workspace switch 1");
}

void Startup::setupTheme() {
    execute("theme tokyo");
}

void Startup::runDashboardTerminal() {
    // Detached so the daemon does not block on the terminal's lifetime.
    ShellUtil::executeShellCommand(
        "setsid kitty --hold echo 'Hello Oschdi ^^' "
        ">/dev/null 2>&1 </dev/null &");
}
