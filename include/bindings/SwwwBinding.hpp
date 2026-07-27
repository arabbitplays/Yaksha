#ifndef SWWW_BINDING
#define SWWW_BINDING

#include <memory>
#include <optional>
#include <string>

#include "core/ShellActuator.hpp"

class SwwwBinding {
public:
    explicit SwwwBinding(ShellActuatorHandle shell_actuator);
    ~SwwwBinding() = default;

    void setWallpaper(const std::string& path,
                      const std::optional<std::string>& monitor_name = std::nullopt) const;

private:
    static constexpr const char* TRANSITION_OPTIONS =
        "--transition-type outer --transition-pos top-right --resize crop "
        "--transition-bezier=0.1,0.2,0.4,1.0 --transition-fps=60";

    ShellActuatorHandle shell_actuator;
};

typedef std::shared_ptr<SwwwBinding> SwwwBindingHandle;

#endif // SWWW_BINDING
