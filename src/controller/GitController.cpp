#include "include/controller/GitController.hpp"
#include "util/ShellUtil.hpp"
#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace {
    constexpr const char* GREEN  = "\033[0;32m";
    constexpr const char* RED    = "\033[0;31m";
    constexpr const char* YELLOW = "\033[0;33m";
    constexpr const char* RESET  = "\033[0m";

    std::string colorize(const std::string& text, const char* color) {
        return std::string(color) + text + RESET;
    }
}

std::string GitController::getKeyword() const {
    return "git";
}

std::string GitController::execute(io::CommandHandle &cmd) {
    if (cmd->args.size() < 1) {
        throw std::runtime_error("Expected at least one argument for command " + getKeyword());
    }

    if (cmd->args[0] == "sync") {
        return sync();
    }
    throw std::runtime_error("Command " + getKeyword() + " " + cmd->args[0] + " does not exist!");
}

std::string GitController::sync() {
    // ssh key auth is provided by gnome-keyring's ssh-agent — the systemd
    // user service exports SSH_AUTH_SOCK=$XDG_RUNTIME_DIR/keyring/ssh, and
    // the passphrase is unlocked once at login via PAM
    std::ostringstream out;
    out << "--------------------------\n"
        << "Sync Obsidian Repositories\n";
    out << syncRepo("Second-Brain",
                    "git -C " + home() + "/Resources/Second-Brain") << "\n";
    out << syncRepo("Zettelkasten",
                    "git -C " + home() + "/Resources/Second-Brain/Zettelkasten") << "\n";

    out << "--------------------------\n"
        << "Sync Install Files\n";
    out << syncRepo(".install", "git -C " + home() + "/.install") << "\n";

    out << "--------------------------\n"
        << "Sync Dotfiles\n";
    out << syncDotfiles();

    return out.str();
}

std::string GitController::syncRepo(const std::string& label, const std::string& git_prefix) {
    std::ostringstream out;
    out << "[" << label << "]\n";

    auto is_repo = ShellUtil::executeShellCommandStatus(
        git_prefix + " rev-parse --is-inside-work-tree >/dev/null 2>&1");
    if (is_repo.second != 0) {
        out << colorize("Not a Git repository.", RED);
        return out.str();
    }

    auto pull = ShellUtil::executeShellCommandStatus(git_prefix + " pull 2>&1");
    if (pull.second == 0) {
        out << colorize(" Pull successful ", GREEN) << "\n";
    } else {
        out << pull.first << "\n" << colorize(" Pull failed:", RED) << "\n";
    }

    ShellUtil::executeShellCommandStatus(git_prefix + " add . 2>&1");

    auto staged = ShellUtil::executeShellCommandStatus(
        git_prefix + " diff --cached --quiet");
    if (staged.second == 0) {
        out << colorize(" Nothing to commit.", YELLOW);
        return out.str();
    }

    std::string msg = "sync " + currentDate();
    auto commit = ShellUtil::executeShellCommandStatus(
        git_prefix + " commit -m \"" + msg + "\" 2>&1");
    if (commit.second == 0) {
        out << colorize(" Commit successful", GREEN) << "\n";
    } else {
        out << commit.first << "\n" << colorize(" Commit failed", RED) << "\n";
    }

    auto push = ShellUtil::executeShellCommandStatus(git_prefix + " push 2>&1");
    if (push.second == 0) {
        out << colorize(" Push successful", GREEN);
    } else {
        out << push.first << "\n" << colorize(" Push failed", RED);
    }

    return out.str();
}

std::string GitController::syncDotfiles() {
    // Bare-repo pattern: work-tree is $HOME, git-dir is $HOME/.cfg.
    std::string cfg = "git --git-dir=" + home() + "/.cfg/ --work-tree=" + home();

    std::ostringstream out;
    out << "[dotfiles]\n";

    ShellUtil::executeShellCommandStatus(cfg + " pull 2>&1");
    ShellUtil::executeShellCommandStatus(cfg + " add " + home() + "/.config/hypr 2>&1");
    ShellUtil::executeShellCommandStatus(cfg + " add " + home() + "/.nixos 2>&1");

    std::string msg = "sync " + currentDate();
    auto commit = ShellUtil::executeShellCommandStatus(
        cfg + " commit -a -m \"" + msg + "\" 2>&1");
    if (commit.second == 0) {
        out << colorize(" Commit successful", GREEN) << "\n";
    } else {
        out << commit.first << "\n" << colorize(" Nothing to commit or commit failed", YELLOW) << "\n";
    }

    auto push = ShellUtil::executeShellCommandStatus(
        cfg + " push --set-upstream origin master 2>&1");
    if (push.second == 0) {
        out << colorize(" Push successful", GREEN);
    } else {
        out << push.first << "\n" << colorize(" Push failed", RED);
    }

    return out.str();
}

std::string GitController::currentDate() {
    return ShellUtil::executeShellCommand("date");
}

std::string GitController::home() {
    const char* h = getenv("HOME");
    if (h == nullptr) {
        throw std::runtime_error("HOME environment variable is not set");
    }
    return std::string(h);
}
