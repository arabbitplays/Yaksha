#ifndef WAYBAR_BINDING
#define WAYBAR_BINDING

#include <memory>

#include "core/ShellActuator.hpp"

class WaybarBinding {
public:
    explicit WaybarBinding(ShellActuatorHandle shell_actuator);
    ~WaybarBinding() = default;

    void restart() const;

private:
    ShellActuatorHandle shell_actuator;
};

typedef std::shared_ptr<WaybarBinding> WaybarBindingHandle;

#endif // WAYBAR_BINDING
