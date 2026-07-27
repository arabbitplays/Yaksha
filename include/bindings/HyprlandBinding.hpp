#ifndef HYPRLAND_BINDING
#define HYPRLAND_BINDING

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/ShellActuator.hpp"
#include <logging/LogManager.hpp>

class HyprlandBinding {
public:
    struct MonitorWorkspace {
        std::string monitor_name;
        uint32_t hyprland_workspace_id;
    };

    explicit HyprlandBinding(ShellActuatorHandle shell_actuator);
    ~HyprlandBinding() = default;

    std::vector<std::string> getMonitorNames() const;
    uint32_t getActiveWorkspaceId() const;
    std::string getActiveWindowAddress() const;

    void focusAndOpenWorkspaces(
        const std::vector<MonitorWorkspace>& focuses,
        const std::optional<std::string>& final_focus_monitor = std::nullopt) const;

    void moveWindowToWorkspace(uint32_t workspace_id,
                               const std::string& window_address,
                               bool silent) const;

    void reload() const;

private:
    ShellActuatorHandle shell_actuator;
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<HyprlandBinding>();
};

typedef std::shared_ptr<HyprlandBinding> HyprlandBindingHandle;

#endif // HYPRLAND_BINDING
