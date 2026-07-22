#include "include/controller/WorkspaceController.hpp"
#include "io/CommandParser.hpp"
#include "util/MonitorUtil.hpp"
#include "util/ShellUtil.hpp"
#include <cassert>
#include <stdexcept>
#include <string>

WorkspaceController::WorkspaceController() {
    monitor_names = MonitorUtil::getMonitorNamesForCurrSystem(); 
}

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
        switchWorkspace(io::CommandParser::parseIntArg(cmd->args[1]));
    } else if (cmd->args[0] == "send") { 
        if (cmd->args.size() < 2) {
            throw std::runtime_error("Expected at least two arguments for command " + getKeyword() + " send");
        }
        sendWindow(io::CommandParser::parseIntArg(cmd->args[1]));
    } else if (cmd->args[0] == "mvleft") { 
        moveWindow(-1);
    } else if (cmd->args[0] == "mvright") { 
        moveWindow(1);
    } else if (cmd->args[0] == "getvirtidx") {
        uint32_t idx = getCurrentVirtualIndex();
        return std::to_string(idx);
    } else {
        throw std::runtime_error("Command " + getKeyword() + " " + cmd->args[0] + " does not exist!");
    }

    return "ok";
}

void WorkspaceController::switchWorkspace(uint32_t target_virtual) const {
    std::string focused_monitor = getActiveMonitorName();

    std::string batch = "hyprctl --batch \"";
    for (uint32_t i = 0; i < monitor_names.size(); i++) {
        batch += "dispatch focusmonitor " + monitor_names[i] + " ; ";
        batch += "dispatch workspace " + std::to_string(getWorkspaceId(i + 1, target_virtual)) + " ; ";
    }
    batch += "dispatch focusmonitor " + focused_monitor + "\"";

    ShellUtil::printShellOutput(ShellUtil::executeShellCommand(batch));
}

void WorkspaceController::sendWindow(uint32_t target_virtual) const {
    std::string active_window = getActiveWindowId();
    Workspace workspace = getCurrentWorkspace();
    std::string result = ShellUtil::executeShellCommand("hyprctl dispatch movetoworkspacesilent " + std::to_string(getWorkspaceId(workspace.physical_id, target_virtual)) + ",address:" + active_window);
    ShellUtil::printShellOutput(result);
}

void WorkspaceController::moveWindow(int32_t physical_delta) const {
    assert(physical_delta == -1 || physical_delta == 1);
    std::string active_window = getActiveWindowId();
    Workspace workspace = getCurrentWorkspace();

    // monitors are 1 indexed
    int32_t physical_id = workspace.physical_id + physical_delta;
    physical_id = physical_id == 0 ? monitor_names.size() : physical_id;
    physical_id = physical_id > static_cast<int32_t>(monitor_names.size()) ? 1 : physical_id;

    ShellUtil::executeShellCommand("hyprctl dispatch movetoworkspace " + std::to_string(getWorkspaceId(physical_id, workspace.virtual_id)) + ",address:" + active_window);
}

uint32_t WorkspaceController::getCurrentVirtualIndex() const {
    Workspace workspace = getCurrentWorkspace();
    return workspace.virtual_id;
}

WorkspaceController::Workspace WorkspaceController::getCurrentWorkspace() const {
    uint32_t workspace = ShellUtil::parseStringToInt(ShellUtil::executeShellCommand("hyprctl activeworkspace -j | jq -r '.id'"));
    return {workspace / 10, workspace % 10};
}

std::string WorkspaceController::getActiveWindowId() const {
    return ShellUtil::executeShellCommand("hyprctl activewindow -j | jq -r '.address'");
}

std::string WorkspaceController::getActiveMonitorName() const {
    return ShellUtil::executeShellCommand("hyprctl activeworkspace -j | jq -r '.monitor'");
}
