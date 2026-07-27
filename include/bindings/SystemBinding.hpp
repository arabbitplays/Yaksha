#ifndef SYSTEM_BINDING
#define SYSTEM_BINDING

#include <memory>
#include <string>

#include "core/ShellActuator.hpp"

class SystemBinding {
public:
    explicit SystemBinding(ShellActuatorHandle shell_actuator);
    ~SystemBinding() = default;

    std::string currentDate() const;
    void copyFile(const std::string& src, const std::string& dst) const;

private:
    ShellActuatorHandle shell_actuator;
};

typedef std::shared_ptr<SystemBinding> SystemBindingHandle;

#endif // SYSTEM_BINDING
