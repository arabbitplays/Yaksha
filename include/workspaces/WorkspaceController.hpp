#ifndef WORKSPACE_CONTROLLER
#define WORKSPACE_CONTROLLER

#include "../core/IController.hpp"
#include "../core/ShellActuator.hpp"

#include "WorkspaceService.hpp"

class WorkspaceController : public IController {
public:
    explicit WorkspaceController(const ShellActuatorHandle& shell_actuator);
    ~WorkspaceController() override = default;

    [[nodiscard]] std::string getKeyword() const override;
    std::string execute(io::CommandHandle &cmd) override;

private:

    std::shared_ptr<WorkspaceService> workspace_service;

};

#endif // WORKSPACE_CONTROLLER

