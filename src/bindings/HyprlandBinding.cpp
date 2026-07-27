#include "bindings/HyprlandBinding.hpp"

#include <stdexcept>
#include <utility>

#include "core/util/StringUtil.h"

HyprlandBinding::HyprlandBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

std::vector<std::string> HyprlandBinding::getMonitorNames() const {
    ShellResult result = shell_actuator->executeShellCommand("hyprctl monitors -j | jq -r '.[].name'");
    if (result.status != 0) {
        throw std::runtime_error("hyprctl monitors failed: " + result.response);
    }
    return StringUtil::split(result.response, '\n');
}

uint32_t HyprlandBinding::getActiveWorkspaceId() const {
    ShellResult result = shell_actuator->executeShellCommand("hyprctl activeworkspace -j | jq -r '.id'");
    if (result.status != 0) {
        throw std::runtime_error("hyprctl activeworkspace failed: " + result.response);
    }
    return static_cast<uint32_t>(std::stoi(result.response));
}

std::string HyprlandBinding::getActiveWindowAddress() const {
    ShellResult result = shell_actuator->executeShellCommand("hyprctl activewindow -j | jq -r '.address'");
    if (result.status != 0) {
        throw std::runtime_error("hyprctl activewindow failed: " + result.response);
    }
    return result.response;
}

void HyprlandBinding::focusAndOpenWorkspaces(
    const std::vector<MonitorWorkspace>& focuses,
    const std::optional<std::string>& final_focus_monitor) const {
    std::string batch = "hyprctl --batch \"";
    for (size_t i = 0; i < focuses.size(); ++i) {
        std::string id_string = std::to_string(focuses[i].hyprland_workspace_id);
        batch += std::format("dispatch focusmonitor {} ; dispatch focusworkspaceoncurrentmonitor {}", focuses[i].monitor_name, id_string);
        const bool needs_separator = (i + 1 < focuses.size()) || final_focus_monitor.has_value();
        if (needs_separator) {
            batch += " ; ";
        }
    }
    if (final_focus_monitor.has_value()) {
        batch += "dispatch focusmonitor " + *final_focus_monitor;
    }
    batch += "\"";
    shell_actuator->executeShellCommand(batch);
}

void HyprlandBinding::moveWindowToWorkspace(uint32_t workspace_id,
                                            const std::string& window_address,
                                            bool silent) const {
    const std::string dispatch = silent ? "movetoworkspacesilent" : "movetoworkspace";
    shell_actuator->executeShellCommand(
        "hyprctl dispatch " + dispatch + " " + std::to_string(workspace_id) + ",address:" + window_address);
}

void HyprlandBinding::reload() const {
    shell_actuator->executeShellCommand("hyprctl reload");
}
