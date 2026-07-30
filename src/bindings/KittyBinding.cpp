#include "bindings/KittyBinding.hpp"

#include <string>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <spawn.h>
#include <cstdio>


KittyBinding::KittyBinding(ShellActuatorHandle shell_actuator)
    : shell_actuator(std::move(shell_actuator))
{
}

void KittyBinding::reload() const
{
    shell_actuator->executeShellCommand("kill -USR1 $(pidof kitty)");
}

void KittyBinding::launchPipedTerminal(const std::string& fifo_name,
                                       const std::string& tty_report_fifo) const
{
    // Report the child's tty, wait for the parent to create the data FIFO,
    // then enter the reader loop.
    std::string cmd = "tty > " + tty_report_fifo
                    + "; until [ -p " + fifo_name + " ]; do sleep 0.05; done"
                    + "; while cat " + fifo_name + "; do :; done";
    const char* argv[] = {"kitty", "sh", "-c", cmd.c_str(), nullptr};

    pid_t pid;
    if (posix_spawnp(&pid, "kitty", nullptr, nullptr,
                     const_cast<char* const*>(argv), environ) != 0)
    {
        perror("posix_spawnp");
    }
}
