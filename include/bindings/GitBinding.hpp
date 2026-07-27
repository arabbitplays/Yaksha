#ifndef GIT_BINDING
#define GIT_BINDING

#include <memory>
#include <string>

#include "core/ShellActuator.hpp"

class GitBinding {
public:
    explicit GitBinding(ShellActuatorHandle shell_actuator);
    ~GitBinding() = default;

    bool pull(const std::string& prefix) const;
    bool add(const std::string& prefix, const std::string& path) const;
    bool hasStagedChanges(const std::string& prefix) const;
    bool commit(const std::string& prefix, const std::string& message) const;
    bool push(const std::string& prefix) const;
    bool isRepository(const std::string& prefix) const;

private:
    ShellActuatorHandle shell_actuator;
};

typedef std::shared_ptr<GitBinding> GitBindingHandle;

#endif // GIT_BINDING
