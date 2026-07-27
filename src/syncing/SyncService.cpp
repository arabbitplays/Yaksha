#include "../../include/syncing/SyncService.hpp"

#include <utility>

#include "syncing/model/GitSyncResult.hpp"

SyncService::SyncService(const ShellActuatorHandle& shell_actuator, GitBindingHandle git_binding)
    : shell_actuator(shell_actuator), git_binding(std::move(git_binding))
{
    for (const auto& path : {"/Resources/Second-Brain", "/Resources/Second-Brain/Zettelkasten", "/.install"})
    {
        try
        {
            addGitRepositoryToSync(path);
        }
        catch (const std::invalid_argument& e)
        {
            logger->warn(std::string("Skipping repository: ") + e.what());
        }
    }
}

void SyncService::addGitRepositoryToSync(const std::filesystem::path& path)
{
    auto git_repository = std::make_shared<GitRepository>(path);
    if (!git_binding->isRepository(getGitPrefix(git_repository)))
    {
        throw std::invalid_argument("'" + path.string() + "' is not a git repository");
    }
    syncedRepositories.push_back(git_repository);
}

std::vector<GitSyncResult> SyncService::syncGitRepositories()
{
    // ssh key auth is provided by gnome-keyring's ssh-agent — the systemd
    // user service exports SSH_AUTH_SOCK=$XDG_RUNTIME_DIR/keyring/ssh, and
    // the passphrase is unlocked once at login via PAM
    std::vector<GitSyncResult> result{};
    for (auto repository : syncedRepositories)
    {
        result.push_back(syncGitRepository(repository->name, getGitPrefix(repository), {"."}));
    }
    return result;
}

GitSyncResult SyncService::syncConfigFiles()
{
    // Bare-repo pattern: work-tree is $HOME, git-dir is $HOME/.cfg.
    std::string git_prefix = "git --git-dir=" + home() + "/.cfg/ --work-tree=" + home();
    return syncGitRepository("dotfiles", git_prefix, {
        home() + "/.config/hypr",
        home() + "/.nixos",
    });
}

GitSyncResult SyncService::syncGitRepository(const std::string& name, const std::string& git_prefix,
                                             std::vector<std::string> add_paths)
{
    if (!git_binding->pull(git_prefix))
    {
        return {name, PullResult::FAILED, PushResult::FAILED};
    }

    for (const auto& add_path : add_paths)
    {
        if (!git_binding->add(git_prefix, add_path))
        {
            return {name, PullResult::SUCCESS, PushResult::FAILED};
        }
    }

    if (!git_binding->hasStagedChanges(git_prefix))
    {
        return {name, PullResult::SUCCESS, PushResult::NOTHING_TO_COMMIT};
    }

    if (!git_binding->commit(git_prefix, getSyncCommitMessage()))
    {
        return {name, PullResult::SUCCESS, PushResult::FAILED};
    }

    if (!git_binding->push(git_prefix))
    {
        return {name, PullResult::SUCCESS, PushResult::FAILED};
    }

    return {name, PullResult::SUCCESS, PushResult::SUCCESS};
}

std::string SyncService::getGitPrefix(const GitRepositoryHandle& repository)
{
    return "git -C " + home() + repository->path.string();
}

std::string SyncService::getSyncCommitMessage() const
{
    return "Sync " + currentDate();
}

std::string SyncService::currentDate() const
{
    return shell_actuator->executeShellCommand("date").response;
}

std::string SyncService::home()
{
    const char* h = getenv("HOME");
    if (h == nullptr)
    {
        throw std::runtime_error("HOME environment variable is not set");
    }
    return std::string(h);
}
