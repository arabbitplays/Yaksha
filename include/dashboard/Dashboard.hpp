#ifndef YAKSHA_DASHBOARD_HPP
#define YAKSHA_DASHBOARD_HPP
#include <thread>

#include "bindings/KittyBinding.hpp"
#include <terminal_renderer/TerminalRenderer.hpp>

class Dashboard
{
public:
    Dashboard(const std::shared_ptr<KittyBinding>& kitty_binding);
    ~Dashboard();

    void launch();
    void runDashboard();

private:
    std::shared_ptr<KittyBinding> kitty_binding;
    TerminalRenderer::RendererHandle terminal_renderer;

    std::atomic<bool> running;
    std::thread dashboard_thread;

    const std::string TERMINAL_PIPE_NAME = "desktop_manager_dashboard_pipe";
};

#endif //YAKSHA_DASHBOARD_HPP