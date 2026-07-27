#ifndef KITTY_BINDING
#define KITTY_BINDING

#include <memory>

#include "core/ShellActuator.hpp"

class KittyBinding {
public:
    explicit KittyBinding(ShellActuatorHandle shell_actuator);
    ~KittyBinding() = default;

    void reload() const;
    void launchDashboard() const;

private:
    ShellActuatorHandle shell_actuator;
};

typedef std::shared_ptr<KittyBinding> KittyBindingHandle;

#endif // KITTY_BINDING
