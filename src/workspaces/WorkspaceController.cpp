#include "../../include/workspaces/WorkspaceController.hpp"
#include "io/CommandParser.hpp"
#include <stdexcept>
#include <string>
#include <utility>

WorkspaceController::WorkspaceController(std::shared_ptr<WorkspaceService> workspace_service)
    : workspace_service(std::move(workspace_service)) {}

std::string WorkspaceController::getKeyword() const {
    return "workspace";
}

std::string WorkspaceController::execute(io::CommandHandle &cmd) {
    if (cmd->args.size() < 1) {
        throw std::runtime_error("Expected at least one argument for command " + getKeyword());
    }

    if (cmd->args[0] == "switch") {
        if (cmd->args.size() < 2) {
            throw std::runtime_error("Expected at least two arguments for command " + getKeyword() + " switch");
        }
        workspace_service->switchWorkspace(io::CommandParser::parseIntArg(cmd->args[1]) - 1);
    } else if (cmd->args[0] == "send") {
        if (cmd->args.size() < 2) {
            throw std::runtime_error("Expected at least two arguments for command " + getKeyword() + " send");
        }
        workspace_service->sendWindow(io::CommandParser::parseIntArg(cmd->args[1]) - 1);
    } else if (cmd->args[0] == "mvleft") {
        workspace_service->moveWindow(LEFT);
    } else if (cmd->args[0] == "mvright") {
        workspace_service->moveWindow(RIGHT);
    } else if (cmd->args[0] == "getvirtidx") {
        uint32_t virtual_id_1based = workspace_service->getActiveWorkspace().virtual_id + 1;
        return std::to_string(virtual_id_1based);
    } else {
        throw std::runtime_error("Command " + getKeyword() + " " + cmd->args[0] + " does not exist!");
    }

    return "ok";
}
