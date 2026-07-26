
#include "include/DesktopManager.hpp"
#include "../include/core/IController.hpp"
#include "controller/GitController.hpp"
#include "../include/theming/ThemeController.hpp"
#include "controller/WorkspaceController.hpp"
#include "io/CommandParser.hpp"
#include "startup/Startup.hpp"
#include <cmath>
#include <concepts>
#include <memory>

#include <ostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <util/MonitorUtil.hpp>

DesktopManager::DesktopManager(bool dev_mode) {
    socket_path = dev_mode
        ? "/tmp/desktop-manager-dev.sock"
        : std::string(getenv("XDG_RUNTIME_DIR")) + "/desktop-manager/desktop-manager.sock"; 
    addController<ThemeController>();
    addController<WorkspaceController>();
    addController<GitController>();

    if (!dev_mode) {
        initDesktopEnvironment();
    }
}

void DesktopManager::initDesktopEnvironment() {
    LOGGER->info("Initialising Desktop Environment");
    Startup startup([this](const std::string& cmd) { return executeCommand(cmd); });
    startup.setupWorkspaces();
    startup.setupTheme();
    startup.runDashboardTerminal();
    LOGGER->info("Finished initialising Desktop Environment");
}

void DesktopManager::run() {
    int server = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server < 0) { perror("socket"); exit(1); }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path.c_str()); 
    unlink(addr.sun_path);

    if (bind(server, (sockaddr*)&addr, sizeof(addr)) < 0) { 
        perror("bind");
        exit(1);
    }

    if (listen(server, 5) < 0) {
        perror("listen");
        exit(1);
    }

    LOGGER->info("Listening on socket " + socket_path);

    while (true) {
        int client = accept(server, nullptr, nullptr);
        char buf[256];
        int n = read(client, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = 0;
            std::string response = executeCommand(std::string(buf));
            write(client, response.c_str(), response.size());

            LOGGER->debug("Response:\n" + response);
        }
        close(client);
    }
}

std::string DesktopManager::executeCommand(const std::string& cmd_string) const {
    LOGGER->info("Received command: " + cmd_string);

    try {
        io::CommandParser parser;
        io::CommandHandle cmd = parser.parseCommand(cmd_string);

        if (!controllers.contains(cmd->keyword))
            return "Error: Controller with keyword " + cmd->keyword + " does not exist";

        return controllers.at(cmd->keyword)->execute(cmd);
    } catch (const std::exception& e) {
        std::string msg = "Error while executing command: " + std::string(e.what());
        // TODO why returning?
        return msg;
    }
}
