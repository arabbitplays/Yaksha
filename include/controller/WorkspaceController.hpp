#ifndef WORKSPACE_CONTROLLER
#define WORKSPACE_CONTROLLER

#include "controller/IController.hpp"
#include <cstdint>
class WorkspaceController : public IController {
public:
    WorkspaceController();
    ~WorkspaceController() = default;

    std::string getKeyword() const override;
    std::string execute(io::CommandHandle &cmd) override;

private:
    struct Workspace {
        uint32_t physical_id;
        uint32_t virtual_id;
    };

    void switchWorkspace(uint32_t target_virtual) const;
    void sendWindow(uint32_t target_virtual) const;
    void moveWindow(int32_t physical_delta) const;
    uint32_t getCurrentVirtualIndex() const;

    Workspace getCurrentWorkspace() const;
    std::string getActiveWindowId() const;
    std::string getActiveMonitorName() const;

    static uint32_t getWorkspaceId(uint32_t physical, uint32_t virt) {
        return physical * 10 + virt;
    }

    std::vector<std::string> monitor_names;
};

#endif // WORKSPACE_CONTROLLER

