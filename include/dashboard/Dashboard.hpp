#ifndef YAKSHA_DASHBOARD_HPP
#define YAKSHA_DASHBOARD_HPP
#include <thread>

#include "bindings/KittyBinding.hpp"
#include <terminal_renderer/TerminalRenderer.hpp>

#include "syncing/SyncService.hpp"
#include "widgets/BannerWidget.hpp"
#include "widgets/SyncWidget.hpp"

class Dashboard
{
public:
    Dashboard(const std::shared_ptr<SyncService> sync_service, const std::shared_ptr<KittyBinding>& kitty_binding);
    ~Dashboard();

    void launch();
    void runDashboard();

private:
    void createWidgets(const std::shared_ptr<SyncService>& sync_service);

    std::shared_ptr<KittyBinding> kitty_binding;
    RendererHandle terminal_renderer;

    std::shared_ptr<BannerWidget> banner_widget;
    std::shared_ptr<SyncWidget> sync_widget;

    std::atomic<bool> running;
    std::thread dashboard_thread;

    const std::string TERMINAL_PIPE_NAME = "desktop_manager_dashboard_pipe";
};

#endif //YAKSHA_DASHBOARD_HPP