#include "bindings/GitBinding.hpp"

#include <utility>

GitBinding::GitBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator)) {}

bool GitBinding::pull(const std::string& prefix) const {
    return shell_actuator->executeShellCommand(prefix + " pull").status == 0;
}

bool GitBinding::add(const std::string& prefix, const std::string& path) const {
    return shell_actuator->executeShellCommand(prefix + " add " + path).status == 0;
}

bool GitBinding::hasStagedChanges(const std::string& prefix) const {
    // `git diff --cached --quiet` exits 0 when nothing is staged, 1 when there are staged changes.
    return shell_actuator->executeShellCommand(prefix + " diff --cached --quiet").status != 0;
}

bool GitBinding::commit(const std::string& prefix, const std::string& message) const {
    return shell_actuator->executeShellCommand(prefix + " commit -a -m \"" + message + "\"").status == 0;
}

bool GitBinding::push(const std::string& prefix) const {
    return shell_actuator->executeShellCommand(prefix + " push").status == 0;
}

bool GitBinding::isRepository(const std::string& prefix) const {
    return shell_actuator->executeShellCommandSilent(prefix + " rev-parse --is-inside-work-tree") == 0;
}
