#ifndef DESKTOP_MANAGER_SYNCSERVICE_HPP
#define DESKTOP_MANAGER_SYNCSERVICE_HPP
#include <vector>

#include "bindings/GitBinding.hpp"
#include "bindings/SystemBinding.hpp"
#include <logging/LogManager.hpp>
#include "model/GitRepository.hpp"
#include "model/GitSyncResult.hpp"

class SyncService
{
public:
    SyncService(GitBindingHandle git_binding, SystemBindingHandle system_binding);
    ~SyncService() = default;

    void addGitRepositoryToSync(const std::filesystem::path& path);
    GitSyncResult syncConfigFiles();
    std::vector<GitSyncResult> syncGitRepositories();

private:
    GitSyncResult syncGitRepository(const std::string& name, const std::string& git_prefix, std::vector<std::string> add_paths);

    static std::string getGitPrefix(const GitRepositoryHandle& repository);
    std::string getSyncCommitMessage() const;
    static std::string home();

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<SyncService>();
    GitBindingHandle git_binding;
    SystemBindingHandle system_binding;

    std::vector<GitRepositoryHandle> syncedRepositories{};
};


#endif //DESKTOP_MANAGER_SYNCSERVICE_HPP
