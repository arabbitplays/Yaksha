#ifndef DESKTOP_MANAGER_SYNCCONTROLLER_HPP
#define DESKTOP_MANAGER_SYNCCONTROLLER_HPP

#include "SyncService.hpp"
#include "../core/IController.hpp"
#include "../core/ShellActuator.hpp"

class SyncController : public IController {
public:
    explicit SyncController(const ShellActuatorHandle& shell_actuator);
    ~SyncController() override = default;

    std::string getKeyword() const override;
    std::string execute(io::CommandHandle &cmd) override;

private:
    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<SyncController>();
    std::shared_ptr<SyncService> sync_service;
};

#endif // DESKTOP_MANAGER_SYNCCONTROLLER_HPP
