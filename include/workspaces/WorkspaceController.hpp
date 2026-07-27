#ifndef WORKSPACE_CONTROLLER
#define WORKSPACE_CONTROLLER

#include <memory>

#include "../core/IController.hpp"
#include "WorkspaceService.hpp"

class WorkspaceController : public IController {
public:
    explicit WorkspaceController(std::shared_ptr<WorkspaceService> workspace_service);
    ~WorkspaceController() override = default;

    [[nodiscard]] std::string getKeyword() const override;
    std::string execute(io::CommandHandle &cmd) override;

private:
    std::shared_ptr<WorkspaceService> workspace_service;
};

#endif // WORKSPACE_CONTROLLER
