#include "../../include/syncing/SyncController.hpp"
#include "../../include/syncing/util/GitSyncResultFormatter.hpp"
#include <sstream>
#include <stdexcept>

SyncController::SyncController(const ShellActuatorHandle& shell_actuator)
{
    sync_service = std::make_shared<SyncService>(shell_actuator);
}

std::string SyncController::getKeyword() const {
    return "sync";
}

std::string SyncController::execute(io::CommandHandle &cmd) {
    if (cmd->args.size() < 1) {
        throw std::runtime_error("Expected at least one argument for command " + getKeyword());
    }

    if (cmd->args[0] == "all") {
        auto results = sync_service->syncGitRepositories();
        results.push_back(sync_service->syncConfigFiles());

        std::ostringstream out;
        for (size_t i = 0; i < results.size(); ++i) {
            if (i > 0) out << "\n";
            out << GitSyncResultFormatter::format(results[i]);
        }
        return out.str();
    }
    throw std::runtime_error("Command " + getKeyword() + " " + cmd->args[0] + " does not exist!");
}

