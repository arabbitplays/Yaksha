#include "../../include/syncing/SyncController.hpp"
#include "../../include/syncing/util/GitSyncResultFormatter.hpp"
#include <sstream>
#include <stdexcept>
#include <utility>

SyncController::SyncController(std::shared_ptr<SyncService> sync_service)
    : sync_service(std::move(sync_service)) {}

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
