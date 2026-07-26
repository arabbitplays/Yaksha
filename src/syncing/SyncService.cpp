#include "../../include/syncing/SyncService.hpp"

#include "syncing/model/GitSyncResult.hpp"

SyncService::SyncService(const ShellActuatorHandle& shell_actuator) : shell_actuator(shell_actuator)
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
    if (!isGitRepository(git_repository))
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
    ShellResult pull_result = shell_actuator->executeShellCommand(git_prefix + " pull");
    if (pull_result.status != 0)
    {
        return {name, PullResult::FAILED, PushResult::FAILED};
    }

    for (const auto& add_path : add_paths)
    {
        ShellResult add_result = shell_actuator->executeShellCommand(git_prefix + " add " + add_path);
        if (add_result.status != 0)
        {
            return {name, PullResult::SUCCESS, PushResult::FAILED};
        }
    }

    ShellResult staged_result = shell_actuator->executeShellCommand(
        git_prefix + " diff --cached --quiet");
    if (staged_result.status == 0)
    {
        return {name, PullResult::SUCCESS, PushResult::NOTHING_TO_COMMIT};
    }

    auto commit_result = shell_actuator->executeShellCommand(
        git_prefix + " commit -a -m \"" + getSyncCommitMessage() + "\"");
    if (commit_result.status != 0)
    {
        return {name, PullResult::SUCCESS, PushResult::FAILED};
    }

    ShellResult push_result = shell_actuator->executeShellCommand(git_prefix + " push");
    if (push_result.status != 0)
    {
        return {name, PullResult::SUCCESS, PushResult::FAILED};
    }

    return {name, PullResult::SUCCESS, PushResult::SUCCESS};
}

std::string SyncService::getGitPrefix(const GitRepositoryHandle& repository)
{
    return "git -C " + home() + repository->path.string();
}

bool SyncService::isGitRepository(const GitRepositoryHandle& repository)
{
    std::string git_prefix = getGitPrefix(repository);
    int32_t is_repo_status = shell_actuator->executeShellCommandSilent(
        git_prefix + " rev-parse --is-inside-work-tree");
    return is_repo_status == 0;
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
