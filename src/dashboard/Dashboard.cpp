#include "../../include/dashboard/Dashboard.hpp"

#include "bindings/KittyBinding.hpp"

#include "terminal_renderer/transport/PipeTransport.hpp"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

#include "dashboard/widgets/BannerWidget.hpp"
#include "syncing/SyncService.hpp"
#include "terminal_renderer/builder/RendererBuilder.hpp"
#include "terminal_renderer/builder/SceneBuilder.hpp"
#include "terminal_renderer/builder/SceneExample.hpp"
#include "terminal_renderer/nodes/layouts/LayoutNode.hpp"

Dashboard::Dashboard(const std::shared_ptr<SyncService> sync_service, const std::shared_ptr<KittyBinding>& kitty_binding) : kitty_binding(kitty_binding)
{
    createWidgets(sync_service);
}

void Dashboard::createWidgets(const std::shared_ptr<SyncService>& sync_service)
{
    banner_widget = std::make_shared<BannerWidget>();
    sync_widget = std::make_shared<SyncWidget>(sync_service);
    logging_widget = std::make_shared<LoggingWidget>();
}

Dashboard::~Dashboard()
{
    running = false;
    if (dashboard_thread.joinable())
    {
        dashboard_thread.join();
    }
}

void Dashboard::launch()
{
    const std::string tty_report_fifo = TERMINAL_PIPE_NAME + "_tty";

    // The report FIFO must exist before kitty redirects `tty` into it,
    // otherwise the redirection would create a regular file.
    // The data FIFO is created later by PipeTransport; kitty waits for it.
    if (mkfifo(tty_report_fifo.c_str(), 0600) == -1 && errno != EEXIST)
    {
        throw std::runtime_error(
            "Failed to create tty report fifo: " + std::string(std::strerror(errno)));
    }

    kitty_binding->launchPipedTerminal(TERMINAL_PIPE_NAME, tty_report_fifo);

    // Blocks until the child opens the report fifo for writing.
    std::ifstream in(tty_report_fifo);
    std::string tty_path;
    std::getline(in, tty_path);
    in.close();
    ::unlink(tty_report_fifo.c_str());

    if (tty_path.empty())
    {
        throw std::runtime_error("Peer terminal did not report a tty path");
    }

    auto scene = SceneBuilder::scene()
        .addChild(SceneBuilder::horizontalLayout()
            .addChild(banner_widget)
            .addChild(sync_widget).build())
        .addChild(logging_widget)
        .build();
    terminal_renderer = RendererBuilder()
                        .transport(std::make_shared<PipeTransport>(TERMINAL_PIPE_NAME, tty_path))
                        .scene(scene)
                        .build();

    running = true;
    dashboard_thread = std::thread(&Dashboard::runDashboard, this);
}

void Dashboard::runDashboard()
{
    try
    {
        terminal_renderer->start();
        while (running.load(std::memory_order_relaxed))
        {
            terminal_renderer->render();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception& e)
    {
        logger->warn(std::string("Dashboard rendering loop stopped due to exception: ") + e.what());
        running = false;
    }
}
