#ifndef DESKTOP_MANAGER_SYNCSERVICE_HPP
#define DESKTOP_MANAGER_SYNCSERVICE_HPP
#include <vector>

#include "core/ShellActuator.hpp"
#include "logging/include/logging/LogManager.hpp"
#include "logging/logger/Logger.hpp"
#include "model/GitRepository.hpp"
#include "model/GitSyncResult.hpp"


class SyncService
{
public:
    explicit SyncService(const ShellActuatorHandle& shell_actuator);
    ~SyncService() = default;

    void addGitRepositoryToSync(const std::filesystem::path& path);
    GitSyncResult syncConfigFiles();
    std::vector<GitSyncResult> syncGitRepositories();

private:
    bool isGitRepository(const GitRepositoryHandle& repository);
    GitSyncResult syncGitRepository(const std::string& name, const std::string& git_prefix, std::vector<std::string> add_paths);

    std::string getGitPrefix(const GitRepositoryHandle& repository);
    std::string getSyncCommitMessage() const;
    std::string currentDate() const;
    static std::string home();

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<SyncService>();
    ShellActuatorHandle shell_actuator;

    std::vector<GitRepositoryHandle> syncedRepositories{};

};


#endif //DESKTOP_MANAGER_SYNCSERVICE_HPP