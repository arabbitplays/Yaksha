#ifndef YAKSHA_MONITORSTATE_HPP
#define YAKSHA_MONITORSTATE_HPP
#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "Monitor.hpp"
#include "MonitorLayout.hpp"
#include "WindowMovement.hpp"
#include "workspaces/MonitorLayoutFactory.hpp"

struct MonitorState
{
    MonitorState()
    {
        monitor_layout = MonitorLayoutFactory::getMonitorLayoutForCurrSystem();
    }

    std::unordered_map<std::string, MonitorHandle> monitors{};
    MonitorLayout monitor_layout;
    uint32_t current_virtual_id = 0;

    std::vector<MonitorHandle> getConnectedMonitors() const
    {
        std::vector<MonitorHandle> result;
        result.reserve(monitors.size());
        for (const auto& monitor : monitors | std::views::values)
        {
            result.push_back(monitor);
        }
        return result;
    }

    MonitorHandle getMonitorById(const uint32_t physical_id) const
    {
        for (const auto& monitor : monitors | std::views::values)
        {
            if (monitor->physical_id == physical_id)
            {
                return monitor;
            }
        }
        throw std::invalid_argument("No monitor with physical id " + std::to_string(physical_id) + " connected");
    }

    std::optional<MonitorHandle> getAdjacentMonitor(const MonitorHandle& monitor, const WindowMovement movement) const
    {
        std::optional<MonitorHandle> result = std::nullopt;
        std::string curr_monitor = monitor->name;
        for (uint32_t i = 0; i < 10; ++i)
        {
            std::optional<std::string> adjacent_monitor = monitor_layout.getAdjacentMonitorName(curr_monitor, movement);
            if (!adjacent_monitor.has_value()) // movement is not registered
                return std::nullopt;

            if (monitors.contains(adjacent_monitor.value())) // monitor is connected
            {
                result = monitors.at(adjacent_monitor.value());
                break;
            }

            // monitor is currently not connected, try the next in the same dir
            curr_monitor = adjacent_monitor.value();
        }

        return result;
    }
};

typedef std::shared_ptr<MonitorState> MonitorStateHandle;

#endif //YAKSHA_MONITORSTATE_HPP
