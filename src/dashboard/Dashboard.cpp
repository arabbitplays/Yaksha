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
#include "terminal_renderer/builder/RendererBuilder.hpp"
#include "terminal_renderer/builder/SceneBuilder.hpp"
#include "terminal_renderer/builder/SceneExample.hpp"
#include "terminal_renderer/nodes/layouts/LayoutNode.hpp"

Dashboard::Dashboard(const std::shared_ptr<KittyBinding>& kitty_binding) : kitty_binding(kitty_binding)
{
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

    auto banner = std::make_shared<BannerWidget>();
    auto scene = SceneBuilder::scene().addChild(banner).build();
    terminal_renderer = RendererBuilder()
                        .transport(std::make_shared<PipeTransport>(TERMINAL_PIPE_NAME, tty_path))
                        .scene(scene)
                        .build();

    running = true;
    dashboard_thread = std::thread(&Dashboard::runDashboard, this);
}

void Dashboard::runDashboard()
{
    while (running.load(std::memory_order_relaxed))
    {
        terminal_renderer->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
